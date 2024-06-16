---
title: Basic Concepts of USD Maya
---

USD Maya is a plugin for Autodesk Maya that provides a bridge to the Universal Scene Description (USD) framework developed by Pixar. It allows for the import and export of USD data to and from the Maya scene, enabling artists to leverage USD's powerful collaborative and non-destructive workflows within Maya. The plugin also provides a set of Maya nodes that represent USD primitives, allowing for the manipulation of USD data directly within Maya.

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes/ProxyUsdGeomCamera.cpp" line="106">

---

# USD Maya Code Usage

This is an example of how USD Maya is used in the codebase. The `getInternalValue` function is a part of the `ProxyUsdGeomCamera` class, which is a Maya node that represents a USD camera in the Maya scene. This function is responsible for translating data from USD to Maya when the Maya node's attributes are queried.

```c++
// USD -> Maya
bool ProxyUsdGeomCamera::getInternalValue(const MPlug& plug, MDataHandle& dataHandle)
{
    const float mm_to_inches = 0.0393701f;

    MStatus status;
    bool    handledAttribute = false;

    UsdTimeCode usdTime(getTime());

    const UsdGeomCamera camera(getCamera());
    if (!camera)
        return false;

    if (plug == m_nearClipPlane) {
        const UsdAttribute attr(camera.CreateClippingRangeAttr());
        if (attr.GetVariability() != SdfVariability::SdfVariabilityVarying)
            usdTime = UsdTimeCode::Default();
        GfVec2f value;
        if (attr.Get(&value, usdTime)) {
            dataHandle.setMDistance(MDistance(value[0], MDistance::kCentimeters));
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes/ProxyShapeSelection.cpp" line="297">

---

The `createMayaNode` function is another example of how USD Maya is used. This function is responsible for creating a new Maya node that represents a USD prim. It does this by translating the data from the USD prim to the corresponding attributes on the Maya node.

```c++
static void createMayaNode(
    const UsdPrim& usdPrim,
    MObject&       node,
    const MObject& parentNode,
    MDagModifier&  modifier,
    MDGModifier*   modifier2,
    const MPlug&   outStage,
    const MPlug&   outTime,
    bool           pushToPrim,
    bool           readAnimatedValues)
{
    MFnDagNode fn;

    bool isUsdXFormable = usdPrim.IsA<UsdGeomXformable>() && !usdPrim.IsInstanceProxy();

    enum MayaNodeTypeCreated
    {
        customMayaTransforn,
        AL_USDMayaScope,
        AL_USDMayaTransform
    };
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes/Scope.cpp" line="147">

---

The `boundingBox` function in the `Scope` class is an example of how USD Maya can be used to integrate USD data into Maya's native systems. This function calculates the bounding box of a USD scope, which is a type of USD prim that can contain other prims, in the Maya scene.

```c++
//----------------------------------------------------------------------------------------------------------------------
MBoundingBox Scope::boundingBox() const
{
    TF_DEBUG(ALUSDMAYA_EVALUATION).Msg("Scope::boundingBox\n");

    MFnDagNode                      fn;
    MPlug                           sourcePlug = inStageDataPlug().source();
    MFnDagNode                      proxyShape(sourcePlug.node());
    AL::usdmaya::nodes::ProxyShape* foundShape
        = (AL::usdmaya::nodes::ProxyShape*)proxyShape.userNode();

    UsdTimeCode usdTime; // Use default time if can't find a connected proxy shape;
    if (foundShape) {
        auto  outTimePlug = foundShape->outTimePlug();
        MTime outTime = outTimePlug.asMTime();
        usdTime = UsdTimeCode(outTime.as(MTime::uiUnit()));
    }

    // Compute Maya bounding box first. Some nodes can contain both Maya and USD boundable
    // descendants.
    MBoundingBox bbox = MPxTransform::boundingBox();
```

---

</SwmSnippet>

# Key Functions Overview

In this section, we will discuss some of the key functions used in the 'maya-usd' repository. These functions play a crucial role in the operation of the plugin and provide a good understanding of how the codebase works.

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes/wrapProxyShape.cpp" line="108">

---

## getUsdPrimFromMayaPath

The `getUsdPrimFromMayaPath` function is used to find a USD prim associated with a Maya node. It takes a string representing the DAG path of the AL_usdmaya_Transform node as an argument and returns the USD prim if found, else an invalid prim. This function is crucial for establishing the relationship between Maya nodes and USD prims.

```c++
    //------------------------------------------------------------------------------------------------------------------
    /// \brief Find Usd prim associated with Maya node.
    /// \param dagPath Dag path of AL_usdmaya_Transform node.
    /// \return The Usd prim if found, else an invalid prim.
    static UsdPrim getUsdPrimFromMayaPath(const std::string& dagPath)
    {
        MStatus        status;
        MSelectionList sel;
        status = sel.add(MString(dagPath.c_str()));
        if (status) {
            MObject node;
            status = sel.getDependNode(0, node);
            if (status) {
                MFnDependencyNode depNode(node);
                if (depNode.typeId() == AL::usdmaya::nodes::Transform::kTypeId
                    || depNode.typeId() == AL::usdmaya::nodes::Scope::kTypeId) {
                    // Get proxy shape
                    auto transform
                        = static_cast<AL::usdmaya::nodes::Transform*>(depNode.userNode());
                    MPlug stageDataPlug(
                        transform->getProxyShape(), AL::usdmaya::nodes::ProxyShape::outStageData());
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes/wrapProxyShape.cpp" line="153">

---

## getMayaPathFromUsdPrim

The `getMayaPathFromUsdPrim` function is used to find a Maya node associated with a USD prim. It takes a ProxyShape and a UsdPrim as arguments and returns the full DAG path of the Maya node if found, else an empty string. This function is important for mapping USD prims back to their corresponding Maya nodes.

```c++
    //------------------------------------------------------------------------------------------------------------------
    /// \brief Find Maya node associated with Usd prim.
    /// \param prim A valid Usd prim.
    /// \return The full dag path of the Maya node if found, else an empty string.
    static std::string getMayaPathFromUsdPrim(const ProxyShape& proxyShape, const UsdPrim& prim)
    {
        return prim.IsValid() ? AL::maya::utils::convert(proxyShape.getMayaPathFromUsdPrim(prim))
                              : std::string();
    }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/fileio/Export.cpp" line="728">

---

## exportSceneHierarchy

The `exportSceneHierarchy` function is used to export the Maya hierarchy at the specified prim path or selection, into the USD format. It takes a MDagPath and a SdfPath as arguments. This function is crucial for exporting the Maya scene into a USD format.

```c++
void Export::exportSceneHierarchy(MDagPath rootPath, SdfPath& defaultPrim)
{
    MDagPath parentPath = rootPath;
    parentPath.pop();
    static const TfToken xformToken("Xform");

    MItDag it(MItDag::kDepthFirst);
    it.reset(rootPath, MItDag::kDepthFirst, MFn::kTransform);

    std::function<void(MDagPath, MFnTransform&, SdfPath&, ReferenceType, bool)> exportShapeProc
        = [this](
              MDagPath      shapePath,
              MFnTransform& fnTransform,
              SdfPath&      usdPath,
              ReferenceType refType,
              bool          inWorldSpace) {
              this->exportShapesCommonProc(shapePath, fnTransform, usdPath, refType, inWorldSpace);
          };

    std::function<bool(MDagPath, MFnTransform&, SdfPath&, bool)> exportTransformFunc
        = [this](
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/fileio/translators/TransformTranslator.cpp" line="804">

---

## copyAttributes

The `copyAttributes` function is used to copy data from the Maya node onto the USD primitive. It takes a MObject, UsdPrim, ExporterParams, MDagPath, and a boolean as arguments. This function is important for ensuring that attributes from the Maya node are correctly transferred to the USD primitive during the export process.

```c++
//----------------------------------------------------------------------------------------------------------------------
MStatus TransformTranslator::copyAttributes(
    const MObject&        from,
    UsdPrim&              to,
    const ExporterParams& params,
    const MDagPath&       path,
    bool                  exportInWorldSpace)
{
    UsdGeomXform xformSchema(to);
    GfVec3f      scale;
    GfVec3f      shear;
    GfVec3f      rotation;
    int32_t      rotateOrder;
    GfVec3f      rotateAxis;
    GfVec3d      translation;
    GfVec3f      scalePivot;
    GfVec3f      rotatePivot;
    GfVec3f      scalePivotTranslate;
    GfVec3f      rotatePivotTranslate;
    bool         inheritsTransform;
    bool         visible;
```

---

</SwmSnippet>

# USD Maya Endpoints

Understanding ExtraDataPlugin and ProxyUsdGeomCamera

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/fileio/translators/ExtraDataPlugin.h" line="79">

---

## ExtraDataPluginAbstract::import

The `import` method in the `ExtraDataPluginAbstract` class is a virtual method that is intended to be overridden by derived classes. This method is called to import a USD prim into the Maya scene. The method takes a USD prim and a Maya object as parameters, and returns a status indicating whether the import was successful.

```c
    /// \brief  Override this method to import a prim into your scene.
    /// \param  prim the usd prim to be imported into maya
    /// \param  node the node that has been imported, on which you wish to import additionl
    /// attributes \return MS::kSuccess if all ok
    virtual MStatus import(const UsdPrim& prim, const MObject& node) { return MS::kSuccess; }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes/ProxyUsdGeomCamera.cpp" line="73">

---

## ProxyUsdGeomCamera::getCamera

The `getCamera` method in the `ProxyUsdGeomCamera` class retrieves the USD geometry camera associated with the proxy. This method returns a `UsdGeomCamera` object, which represents a camera in the USD scene graph.

```c++
UsdGeomCamera ProxyUsdGeomCamera::getCamera() const
{
    MStatus status;
    MPlug   stagePlug(thisMObject(), m_stage);
    MObject stageObject;
    status = stagePlug.getValue(stageObject);
    AL_MAYA_CHECK_ERROR2(status, "Failed to get 'stage' attr");

    if (status == MStatus::kSuccess) {
        // Pull in stage data
        MFnPluginData fnData(stageObject);

        auto* stageData = static_cast<MayaUsdStageData*>(fnData.data());
        if (stageData != nullptr) {
            // Get prim path
            MPlug   pathPlug(thisMObject(), m_path);
            MString path;
            status = pathPlug.getValue(path);
            AL_MAYA_CHECK_ERROR2(status, "Failed to get 'path' attr");

            return UsdGeomCamera(stageData->stage->GetPrimAtPath(SdfPath(path.asChar())));
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
