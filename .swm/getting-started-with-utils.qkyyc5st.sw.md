---
title: Getting started with Utils
---

Utils, short for utilities, in the maya-usd repository refers to a collection of helper functions and classes that are used across the codebase. These utilities provide common functionality that is needed in multiple places, promoting code reuse and reducing redundancy. They are organized into different files based on their functionality. For example, 'meshReadUtils' contains utilities for reading mesh data, 'jointWriteUtils' contains utilities for writing joint and skin data, 'writeUtil' contains helpers for writing USD data, and so on. These utilities are crucial for maintaining clean, efficient, and readable code in the maya-usd repository.

<SwmSnippet path="/lib/mayaUsd/fileio/utils/meshReadUtils.cpp" line="93">

---

# Utils in meshReadUtils.cpp

In `meshReadUtils.cpp`, the utils are encapsulated within an anonymous namespace. The `addCreaseSet` function is an example of a utility function used to add a crease set to a mesh.

```c++
    "USD_faceVaryingLinearInterpolation");

namespace {
bool addCreaseSet(
    const std::string& rootName,
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/writeUtil.cpp" line="641">

---

# Utils in writeUtil.cpp

In `writeUtil.cpp`, the `_IsAnimated` function within an anonymous namespace is a utility function used to check if a given Maya plug is animated.

```c++
namespace {
bool _IsAnimated(const MPlug& p)
{
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/jointWriteUtils.h" line="42">

---

# Utils in jointWriteUtils.h

In `jointWriteUtils.h`, the `UsdMayaJointUtil` namespace contains utility functions for dealing with writing out joint and skin data. Functions like `getJointHierarchyComponents`, `getJointNames`, `getSkeletonPath`, etc., provide utilities for handling joint hierarchies and skin data.

```c
PXR_NAMESPACE_OPEN_SCOPE

// Utilities for dealing with writing out joint and skin data.
namespace UsdMayaJointUtil {
/// Gets all of the components of the joint hierarchy rooted at \p dagPath.
/// The \p skelXformPath will hold the path to a joint that defines
/// the transform of a UsdSkelSkeleton. It may be invalid if no
/// joint explicitly defines that transform.
/// The \p joints array, if provided, will be filled with the ordered set of
/// joint paths, excluding the set of joints described above.
/// The \p jointHierarchyRootPath will hold the common parent path of
/// all of the returned joints.
MAYAUSD_CORE_PUBLIC
void getJointHierarchyComponents(
    const MDagPath&        dagPath,
    MDagPath*              skelXformPath,
    MDagPath*              jointHierarchyRootPath,
    std::vector<MDagPath>* joints = nullptr);

MAYAUSD_CORE_PUBLIC
SdfPath getAnimationPath(const SdfPath& skelPath);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/meshWriteUtils.h" line="38">

---

# Utils in meshWriteUtils.h

In `meshWriteUtils.h`, the `UsdMayaMeshWriteUtils` namespace contains utility functions for dealing with writing USD from Maya mesh/subdiv tags. Functions like `getSkinClusterConnectedToMesh`, `getMeshNormals`, `getSubdivScheme`, etc., provide utilities for handling mesh data and subdivision schemes.

```c
PXR_NAMESPACE_OPEN_SCOPE

class UsdGeomMesh;

// Utilities for dealing with writing USD from Maya mesh/subdiv tags.
namespace UsdMayaMeshWriteUtils {
/**
 * Finds a skinCluster directly connected upstream in the DG to the given mesh.
 *
 * @param mesh              The mesh to search from.
 * @param skinCluster       Storage for the result. If no skinCluster can be found,
 *                          this will be a null `MObject`.
 *
 * @return                  `MStatus::kSuccess` if the operation completed successfully.
 */
MAYAUSD_CORE_PUBLIC
MStatus getSkinClusterConnectedToMesh(const MObject& mesh, MObject& skinCluster);

/**
 * Similar to `getSkinClusterConnectedToMesh`, except that instead of finding a
 * directly-connected skinCluster, it searches the DG upstream of the mesh for any
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/writeUtil.h" line="42">

---

# Utils in writeUtil.h

In `writeUtil.h`, the `UsdMayaWriteUtil` struct contains helper functions for writing USD data. Functions like `WriteUVAsFloat2`, `GetOrCreateUsdAttr`, `SetUsdAttr`, etc., provide utilities for creating and setting USD attributes.

```c
PXR_NAMESPACE_OPEN_SCOPE

class FlexibleSparseValueWriter;
struct UsdMayaJobExportArgs;

/// This struct contains helpers for writing USD (thus reading Maya data).
struct UsdMayaWriteUtil
{
    /// \name Helpers for writing USD
    /// \{

    /// Returns whether the environment setting for writing the TexCoord
    /// types is set to true
    MAYAUSD_CORE_PUBLIC
    static bool WriteUVAsFloat2();

    /// Returns the exported name of a UV set at \p uvIndex according to the renaming rule and the
    /// current set of options.
    ///
    /// \p originalNames is the full array returned by MFnMesh::getUVSetNames().
    /// \p preserveSetNames is as defined by the -preserveUVSetNames export option and stored in
```

---

</SwmSnippet>

# Utility Functions Overview

This section provides an overview of the key utility functions used in the 'maya-usd' repository.

<SwmSnippet path="/lib/mayaUsd/fileio/utils/writeUtil.cpp" line="73">

---

## TF_DEFINE_ENV_SETTING

The 'TF_DEFINE_ENV_SETTING' function is used to define environment settings. It takes three parameters: the setting name, the default value, and a description. This function is used to control the behavior of the program using environment variables.

```c++
PXR_NAMESPACE_OPEN_SCOPE

TF_DEFINE_ENV_SETTING(
    PIXMAYA_WRITE_UV_AS_FLOAT2,
    false,
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/writeUtil.cpp" line="263">

---

## TF_FOR_ALL

The 'TF_FOR_ALL' function is a macro that creates a loop over all elements in a container. It is used here to iterate over 'riAttrs' and get the attribute from the prim if the base name matches 'riAttrNameToken'.

```c++
    TF_FOR_ALL(iter, riAttrs)
    {
        if (iter->GetBaseName() == riAttrNameToken) {
            // Re-get the attribute from the prim so we can return it as a
            // UsdAttribute rather than a UsdProperty.
            return usdPrim.GetAttribute(iter->GetName());
        }
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/adaptor.cpp" line="778">

---

## GetAttribute

The 'GetAttribute' function is used to get an attribute from a prim. It takes a 'TfToken' as an argument which represents the attribute name. If the attribute doesn't exist on the schema, it returns an error.

```c++
UsdMayaAttributeAdaptor UsdMayaSchemaAdaptor::GetAttribute(const TfToken& attrName) const
{
    if (!*this) {
        return UsdMayaAttributeAdaptor();
    }

#if PXR_VERSION < 2308
    SdfAttributeSpecHandle attrDef = _GetAttributeSpec(_schemaDef, attrName);
#else
    UsdPrimDefinition::Attribute attrDef = _GetAttributeDef(_schemaDef, attrName);
#endif

    if (!attrDef) {
        TF_CODING_ERROR(
            "Attribute '%s' doesn't exist on schema '%s'",
            attrName.GetText(),
            _schemaName.GetText());
        return UsdMayaAttributeAdaptor();
    }

    std::string       mayaAttrName = _GetMayaAttrNameOrAlias(attrName);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/xformStack.cpp" line="178">

---

## GetName

The 'GetName' function is used to get the name of an attribute. It is used here to get the name of an 'UsdMayaXformOpClassification' object.

```c++
TfToken const& UsdMayaXformOpClassification::GetName() const { return _sharedData->m_name; }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/writeUtil.h" line="305">

---

## SetAttribute

The 'SetAttribute' function is used to set the value of an attribute. It takes an 'UsdAttribute', a value, and an optional 'UsdTimeCode'. If a 'FlexibleSparseValueWriter' is provided, it uses it to set the attribute value, otherwise, it directly sets the value on the attribute.

```c
    static bool SetAttribute(
        const UsdAttribute&        attr,
        const T&                   value,
        const UsdTimeCode          time = UsdTimeCode::Default(),
        FlexibleSparseValueWriter* valueWriter = nullptr)
    {
        return valueWriter ? valueWriter->SetAttribute(attr, VtValue(value), time)
                           : attr.Set(value, time);
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/meshReadUtils.cpp" line="96">

---

## addCreaseSet

The 'addCreaseSet' function is used to add a crease set to a mesh. It takes a root name, a crease level, a component list, and a status. It creates a crease set and adds it to the crease partition. If there are any issues during this process, it returns false.

```c++
bool addCreaseSet(
    const std::string& rootName,
    double             creaseLevel,
    MSelectionList&    componentList,
    MStatus*           statusOK)
{
    // Crease Set functionality is native to Maya, but undocumented and not
    // directly supported in the API. The below implementation is derived from
    // the editor code in the maya distro at:
    //
    // .../lib/python2.7/site-packages/maya/app/general/creaseSetEditor.py

    MObject creasePartitionObj;
    *statusOK = UsdMayaUtil::GetMObjectByName(MString(":creasePartition"), creasePartitionObj);

    if (creasePartitionObj.isNull()) {
        statusOK->clear();

        // There is no documented way to create a shared node through the C++ API
        const std::string partitionName
            = MGlobal::executeCommandStringResult(
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
