//
// Copyright 2018 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "jointWriter.h"

#include <mayaUsd/fileio/primWriter.h>
#include <mayaUsd/fileio/primWriterRegistry.h>
#include <mayaUsd/fileio/translators/translatorSkel.h>
#include <mayaUsd/fileio/translators/translatorUtil.h>
#include <mayaUsd/fileio/utils/adaptor.h>
#include <mayaUsd/fileio/utils/jointWriteUtils.h>
#include <mayaUsd/fileio/utils/writeUtil.h>
#include <mayaUsd/fileio/writeJobContext.h>
#include <mayaUsd/utils/util.h>

#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/tf/diagnostic.h>
#include <pxr/base/tf/staticTokens.h>
#include <pxr/base/tf/token.h>
#include <pxr/pxr.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/usd/timeCode.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdSkel/animation.h>
#include <pxr/usd/usdSkel/bindingAPI.h>
#include <pxr/usd/usdSkel/root.h>
#include <pxr/usd/usdSkel/skeleton.h>
#include <pxr/usd/usdSkel/utils.h>

#include <maya/MAnimUtil.h>
#include <maya/MDagPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnTransform.h>
#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPxNode.h>
#include <maya/MStatus.h>

#include <vector>

#define CHECK_MSTATUS_AND_CONTINUE(_status)      \
    {                                            \
        MStatus _maya_status = (_status);        \
        if (MStatus::kSuccess != _maya_status) { \
            continue;                            \
        }                                        \
    }

PXR_NAMESPACE_OPEN_SCOPE

PXRUSDMAYA_REGISTER_WRITER(joint, PxrUsdTranslators_JointWriter);
PXRUSDMAYA_REGISTER_ADAPTOR_SCHEMA(joint, UsdSkelSkeleton);

PxrUsdTranslators_JointWriter::PxrUsdTranslators_JointWriter(
    const MFnDependencyNode& depNodeFn,
    const SdfPath&           usdPath,
    UsdMayaWriteJobContext&  jobCtx)
    : UsdMayaPrimWriter(depNodeFn, usdPath, jobCtx)
    , _valid(false)
{
    if (!TF_VERIFY(GetDagPath().isValid())) {
        return;
    }

    const TfToken& exportSkels = _GetExportArgs().exportSkels;
    if (exportSkels != UsdMayaJobExportArgsTokens->auto_
        && exportSkels != UsdMayaJobExportArgsTokens->explicit_) {
        return;
    }

    _skel = UsdSkelSkeleton::Define(GetUsdStage(), usdPath);
    if (!TF_VERIFY(_skel)) {
        return;
    }

    _usdPrim = _skel.GetPrim();
}

/// Gets the world-space rest transform for a single dag path.
static GfMatrix4d _GetJointWorldBindTransform(const MDagPath& dagPath)
{
    // In the Maya skin cluster the REAL bindPose data that matters is what
    // is stored on the skinCluster node in bindPreMatrix. The dagPose node and
    // the bindPose attribute on the joints is not used when doing deformation.
    // The values should match up, but someone could edit a scene so they get out of sync.
    // Get the bindTransform from the skinCluster.

    MFnDagNode        dagNode(dagPath);
    MStatus           status;
    MFnDependencyNode dgNode(dagPath.node(), &status);

    MPlug plugWorldMatrixParent = dagNode.findPlug("worldMatrix", true, &status);
    if (status) {
        unsigned int numInstance = plugWorldMatrixParent.numElements(&status);
        TF_VERIFY(numInstance < 2 && status); // if the skeleton is instanced in Maya then what?
        for (unsigned int instanceIndex = 0; instanceIndex < numInstance; instanceIndex++) {
            MPlug plugWorldMatrix = plugWorldMatrixParent.elementByLogicalIndex(instanceIndex);

            MPlugArray plgsDest;
            plugWorldMatrix.destinations(plgsDest);
            GfMatrix4d result;
            MObject    resultNode;
            bool       hasResult = false;

            for (unsigned int i = 0; i < plgsDest.length(); ++i) {
                MPlug   plgDest = plgsDest[i];
                MObject curNode = plgDest.node();
                if (!curNode.hasFn(MFn::kSkinClusterFilter)) {
                    continue;
                }

                // We should be connected to a matrix[x] plug.
                TF_VERIFY(plgDest.isElement());
                unsigned int      membersIdx = plgDest.logicalIndex();
                MFnDependencyNode fnNode(curNode, &status);
                CHECK_MSTATUS_AND_CONTINUE(status);
                MPlug plgWorldMatrices = fnNode.findPlug("bindPreMatrix", false, &status);
                CHECK_MSTATUS_AND_CONTINUE(status);
                MPlug         plgBindPreMatrix = plgWorldMatrices.elementByLogicalIndex(membersIdx);
                MObject       plgBindMatrixData = plgBindPreMatrix.asMObject();
                MFnMatrixData fnMatrixData(plgBindMatrixData, &status);
                CHECK_MSTATUS_AND_CONTINUE(status);
                MMatrix mayaResult = fnMatrixData.matrix().inverse();

                if (!hasResult) {
                    result = GfMatrix4d(mayaResult.matrix);
                    resultNode = curNode;
                    hasResult = true;
                } else {
                    GfMatrix4d tempResult = GfMatrix4d(mayaResult.matrix);
                    if (!GfIsClose(tempResult, result, 1e-6)) {
                        MFnDependencyNode fnResultNode(resultNode, &status);
                        CHECK_MSTATUS_AND_CONTINUE(status);
                        MString errorMsg;
                        errorMsg += "Joint '";
                        errorMsg += dgNode.name();
                        errorMsg += "' has different bind poses. bindPreMatrix values on ";
                        errorMsg += fnResultNode.name();
                        errorMsg += " and ";
                        errorMsg += fnNode.name();
                        errorMsg += " differ. Using bindPreMatrix from ";
                        errorMsg += fnResultNode.name();
                        errorMsg += ".";
                        MGlobal::displayWarning(errorMsg);
                    }
                }
            }

            if (hasResult) {
                return result;
            }
        }
    }

    // Check if the joint is linked to a bindPose, and attempt to grab the bind transform matrix
    // there.
    MPlug plgMsg = dagNode.findPlug(MPxNode::message, false, &status);
    if (status && plgMsg.isSource()) {
        MPlugArray plgsDest;
        plgMsg.destinations(plgsDest);
        for (unsigned int i = 0; i < plgsDest.length(); ++i) {
            MPlug   plgDest = plgsDest[i];
            MObject curNode = plgDest.node();
            if (!curNode.hasFn(MFn::kDagPose)) {
                continue;
            }

            // We should be connected to a members[x] plug.
            TF_VERIFY(plgDest.isElement());
            unsigned int      membersIdx = plgDest.logicalIndex();
            MFnDependencyNode fnNode(curNode, &status);
            CHECK_MSTATUS_AND_CONTINUE(status);
            MPlug plgWorldMatrices = fnNode.findPlug("worldMatrix", false, &status);
            CHECK_MSTATUS_AND_CONTINUE(status);
            MPlug         plgWorldMatrix = plgWorldMatrices.elementByLogicalIndex(membersIdx);
            MObject       plgWorldMatrixData = plgWorldMatrix.asMObject();
            MFnMatrixData fnMatrixData(plgWorldMatrixData, &status);
            CHECK_MSTATUS_AND_CONTINUE(status);
            MMatrix result = fnMatrixData.matrix();

            return GfMatrix4d(result.matrix);
        }
    }

    // If the dagPose node doesn't have an extra for our joint there could be something useful in
    // the bindPose attribute of the joint. Check there.
    MMatrix restTransformWorld;
    if (UsdMayaUtil::getPlugMatrix(dagNode, "bindPose", &restTransformWorld)) {
        return GfMatrix4d(restTransformWorld.matrix);
    }

    return GfMatrix4d(1);
}

/// Gets world-space bind transforms for all specified dag paths.
static VtMatrix4dArray _GetJointWorldBindTransforms(
    const UsdSkelTopology&       topology,
    const std::vector<MDagPath>& jointDagPaths)
{
    size_t          numJoints = jointDagPaths.size();
    VtMatrix4dArray worldXforms(numJoints);
    for (size_t i = 0; i < jointDagPaths.size(); ++i) {
        worldXforms[i] = _GetJointWorldBindTransform(jointDagPaths[i]);
    }
    return worldXforms;
}

bool _GetLocalTransformForDagPoseMember(
    const MFnDependencyNode& dagPoseDep,
    unsigned int             logicalIndex,
    GfMatrix4d*              xform)
{
    MStatus status;

    MPlug xformMatrixPlug = dagPoseDep.findPlug("xformMatrix");
#ifdef MAYA_ARRAY_ITERATOR_DIFFERENCE_TYPE_SUPPORT
    if (TfDebug::IsEnabled(PXRUSDMAYA_TRANSLATORS)) {
        // As an extra debug sanity check, make sure that the logicalIndex
        // already exists
        MIntArray allIndices;
        xformMatrixPlug.getExistingArrayAttributeIndices(allIndices);
        if (std::find(allIndices.cbegin(), allIndices.cend(), logicalIndex) == allIndices.cend()) {
            TfDebug::Helper().Msg(
                "Warning - attempting to retrieve %s[%u], but that index did not exist yet",
                xformMatrixPlug.name().asChar(),
                logicalIndex);
        }
    }
#endif
    MPlug xformPlug = xformMatrixPlug.elementByLogicalIndex(logicalIndex, &status);
    CHECK_MSTATUS_AND_RETURN(status, false);

    MObject plugObj = xformPlug.asMObject(MDGContext::fsNormal, &status);
    CHECK_MSTATUS_AND_RETURN(status, false);

    MFnMatrixData plugMatrixData(plugObj, &status);
    CHECK_MSTATUS_AND_RETURN(status, false);

    *xform = GfMatrix4d(plugMatrixData.matrix().matrix);
    return true;
}

/// Gets the world-space transform of \p dagPath at the current time.
static GfMatrix4d _GetJointWorldTransform(const MDagPath& dagPath)
{
    // Don't use Maya's built-in getTranslation(), etc. when extracting the
    // transform because:
    // - The rotation won't account for the jointOrient rotation, so
    //   you'd have to query that from MFnIkJoint and combine.
    // - The scale is special on joints because the scale on a parent
    //   joint isn't inherited by children, due to an implicit
    //   (inverse of parent scale) factor when computing joint
    //   transformation matrices.
    // In short, no matter what you do, there will be cases where the
    // Maya joint transform can't be perfectly replicated in UsdSkel;
    // it's much easier to ensure correctness by letting UsdSkel work
    // with raw transform data, and perform its own decomposition later
    // with UsdSkelDecomposeTransforms.

    MStatus status;
    MMatrix mx = dagPath.inclusiveMatrix(&status);
    return status ? GfMatrix4d(mx.matrix) : GfMatrix4d(1);
}

/// Gets the world-space transform of \p dagPath at the current time.
static GfMatrix4d _GetJointLocalTransform(const MDagPath& dagPath)
{
    MStatus      status;
    MFnTransform xform(dagPath, &status);
    if (status) {
        MTransformationMatrix mx = xform.transformation(&status);
        if (status) {
            return GfMatrix4d(mx.asMatrix().matrix);
        }
    }
    return GfMatrix4d(1);
}

/// Computes world-space joint transforms for all specified dag paths
/// at the current time.
static bool _GetJointWorldTransforms(const std::vector<MDagPath>& dagPaths, VtMatrix4dArray* xforms)
{
    xforms->resize(dagPaths.size());
    GfMatrix4d* xformsData = xforms->data();
    for (size_t i = 0; i < dagPaths.size(); ++i) {
        xformsData[i] = _GetJointWorldTransform(dagPaths[i]);
    }
    return true;
}

/// Computes joint-local transforms for all specified dag paths
/// at the current time.
static bool _GetJointLocalTransforms(
    const UsdSkelTopology&       topology,
    const std::vector<MDagPath>& dagPaths,
    const GfMatrix4d&            rootXf,
    VtMatrix4dArray*             localXforms)
{
    VtMatrix4dArray worldXforms;
    if (_GetJointWorldTransforms(dagPaths, &worldXforms)) {

        GfMatrix4d rootInvXf = rootXf.GetInverse();

        VtMatrix4dArray worldInvXforms(worldXforms);
        for (auto& xf : worldInvXforms)
            xf = xf.GetInverse();

        return UsdSkelComputeJointLocalTransforms(
            topology, worldXforms, worldInvXforms, localXforms, &rootInvXf);
    }
    return true;
}

/// Given the list of USD joint names and dag paths, returns the joints that
/// (1) are moved from their rest poses or (2) have animation, if we are going
/// to export animation.
static void _GetAnimatedJoints(
    const VtTokenArray&          usdJointNames,
    const std::vector<MDagPath>& jointDagPaths,
    const VtMatrix4dArray&       restXforms,
    VtTokenArray*                animatedJointNames,
    std::vector<MDagPath>*       animatedJointPaths)
{
    if (!TF_VERIFY(usdJointNames.size() == jointDagPaths.size())) {
        return;
    }

    if (restXforms.size() != usdJointNames.size()) {
        // Either have invalid restXforms or no restXforms at all
        // (the latter happens when a user deletes the dagPose).
        // Must treat all joinst as animated.
        *animatedJointNames = usdJointNames;
        *animatedJointPaths = jointDagPaths;
        return;
    }

    // The resulting vector contains only animated joints or joints not
    // in their rest pose. The order is *not* guaranteed to be the Skeleton
    // order, because UsdSkel allows arbitrary order on SkelAnimation.
    for (size_t i = 0; i < jointDagPaths.size(); ++i) {
        const TfToken&  jointName = usdJointNames[i];
        const MDagPath& dagPath = jointDagPaths[i];

        animatedJointNames->push_back(jointName);
        animatedJointPaths->push_back(dagPath);
    }
}

bool PxrUsdTranslators_JointWriter::_WriteRestState()
{
    // Check if the root joint is the special root joint created
    // for round-tripping UsdSkel data.
    bool haveUsdSkelXform = UsdMayaTranslatorSkel::IsUsdSkeleton(GetDagPath());

    if (!haveUsdSkelXform) {
        // We don't have a joint that represents the Skeleton.
        // This means that the joint hierarchy is originating from Maya.
        // Mark it, so that the exported results can be reimported in
        // a structure-preserving way.
        UsdMayaTranslatorSkel::MarkSkelAsMayaGenerated(_skel);
    }

    UsdMayaJointUtil::getJointHierarchyComponents(
        GetDagPath(), &_skelXformPath, &_jointHierarchyRootPath, &_joints);

    VtTokenArray skelJointNames = UsdMayaJointUtil::getJointNames(
        _joints, GetDagPath(), _GetExportArgs().stripNamespaces, _GetExportArgs().rootMapFunction);
    _topology = UsdSkelTopology(skelJointNames);
    std::string whyNotValid;
    if (!_topology.Validate(&whyNotValid)) {
        TF_CODING_ERROR("Joint topology is invalid: %s", whyNotValid.c_str());
        return false;
    }

    // Setup binding relationships on the instance prim,
    // so that the root xform establishes a skeleton instance
    // with the right transform.
    const UsdSkelBindingAPI binding
        = UsdMayaTranslatorUtil ::GetAPISchemaForAuthoring<UsdSkelBindingAPI>(_skel.GetPrim());

    // Mark the bindings for post processing.

    UsdMayaWriteUtil::SetAttribute(
        _skel.GetJointsAttr(), skelJointNames, UsdTimeCode::Default(), _GetSparseValueWriter());

    SdfPath skelPath = _skel.GetPrim().GetPath();
    _writeJobCtx.MarkSkelBindings(skelPath, skelPath, _GetExportArgs().exportSkels);

    VtMatrix4dArray bindXforms = _GetJointWorldBindTransforms(_topology, _joints);
    UsdMayaWriteUtil::SetAttribute(
        _skel.GetBindTransformsAttr(), bindXforms, UsdTimeCode::Default(), _GetSparseValueWriter());

    // Create something reasonable for rest transforms
    VtMatrix4dArray restXforms;
    GfMatrix4d      rootXf = _GetJointWorldTransform(_jointHierarchyRootPath);
    // Setting the skel rest transform as the inverse of the current joint transform.
    // The inverse of the bind pose would be the ideal scenario here, however joints without a bind
    // pose or not linked to a skin cluster would have the identity and wouldn't be represented
    // correctly.
    if (_GetJointLocalTransforms(_topology, _joints, rootXf, &restXforms)) {
        UsdMayaWriteUtil::SetAttribute(
            _skel.GetRestTransformsAttr(),
            restXforms,
            UsdTimeCode::Default(),
            _GetSparseValueWriter());
    } else {
        TF_WARN("Unable to set rest transforms");
    }

    VtTokenArray animJointNames;
    _GetAnimatedJoints(skelJointNames, _joints, restXforms, &animJointNames, &_animatedJoints);

    if (haveUsdSkelXform) {
        _skelXformAttr = _skel.MakeMatrixXform();
        if (!_GetExportArgs().timeSamples.empty()) {
            MObject node = _skelXformPath.node();
            _skelXformIsAnimated = UsdMayaUtil::isAnimated(node);
        } else {
            _skelXformIsAnimated = false;
        }
    }

    if (!animJointNames.empty()) {

        SdfPath animPath = UsdMayaJointUtil::getAnimationPath(skelPath);
        _skelAnim = UsdSkelAnimation::Define(GetUsdStage(), animPath);

        if (TF_VERIFY(_skelAnim)) {
            _skelToAnimMapper = UsdSkelAnimMapper(skelJointNames, animJointNames);

            UsdMayaWriteUtil::SetAttribute(
                _skelAnim.GetJointsAttr(),
                animJointNames,
                UsdTimeCode::Default(),
                _GetSparseValueWriter());

            binding.CreateAnimationSourceRel().SetTargets({ animPath });
        } else {
            return false;
        }
    }
    return true;
}

/* virtual */
void PxrUsdTranslators_JointWriter::Write(const UsdTimeCode& usdTime)
{
    if (usdTime.IsDefault()) {
        _valid = _WriteRestState();
    }

    if (!_valid) {
        return;
    }

    if ((usdTime.IsDefault() || _skelXformIsAnimated) && _skelXformAttr) {

        // We have a joint which provides the transform of the Skeleton,
        // instead of the transform of a joint in the hierarchy.
        GfMatrix4d localXf = _GetJointLocalTransform(_skelXformPath);
        UsdMayaWriteUtil::SetAttribute(_skelXformAttr, localXf, usdTime, _GetSparseValueWriter());
    }

    // Time-varying step: write the packed joint animation transforms once per
    // time code. We do want to run this @ default time also so that any
    // deviations from the rest pose are exported as the default values on the
    // SkelAnimation.
    if (!_animatedJoints.empty()) {

        if (!_skelAnim) {

            SdfPath animPath = UsdMayaJointUtil::getAnimationPath(_skel.GetPrim().GetPath());

            TF_CODING_ERROR(
                "SkelAnimation <%s> doesn't exist but should "
                "have been created during default-time pass.",
                animPath.GetText());
            return;
        }

        GfMatrix4d rootXf = _GetJointWorldTransform(_jointHierarchyRootPath);

        VtMatrix4dArray localXforms;
        if (_GetJointLocalTransforms(_topology, _joints, rootXf, &localXforms)) {

            // Remap local xforms into the (possibly sparse) anim order.
            VtMatrix4dArray animLocalXforms;
            if (_skelToAnimMapper.Remap(localXforms, &animLocalXforms)) {

                VtVec3fArray translations;
                VtQuatfArray rotations;
                VtVec3hArray scales;
                if (UsdSkelDecomposeTransforms(
                        animLocalXforms, &translations, &rotations, &scales)) {

                    // XXX It is difficult for us to tell which components are
                    // actually animated since we rely on decomposition to get
                    // separate anim components.
                    // In the future, we may want to RLE-compress the data in
                    // PostExport to remove redundant time samples.
                    UsdMayaWriteUtil::SetAttribute(
                        _skelAnim.GetTranslationsAttr(),
                        &translations,
                        usdTime,
                        _GetSparseValueWriter());
                    UsdMayaWriteUtil::SetAttribute(
                        _skelAnim.GetRotationsAttr(), &rotations, usdTime, _GetSparseValueWriter());
                    UsdMayaWriteUtil::SetAttribute(
                        _skelAnim.GetScalesAttr(), &scales, usdTime, _GetSparseValueWriter());
                }
            }
        }
    }
}

/* virtual */
bool PxrUsdTranslators_JointWriter::ExportsGprims() const
{
    // Nether the Skeleton nor its animation sources are gprims.
    return false;
}

/* virtual */
bool PxrUsdTranslators_JointWriter::ShouldPruneChildren() const { return true; }

PXR_NAMESPACE_CLOSE_SCOPE
