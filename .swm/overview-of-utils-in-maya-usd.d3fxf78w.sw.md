---
title: Overview of Utils in Maya-USD
---
Utils, short for utilities, in the maya-usd repository refers to a collection of helper functions and classes that are used across the codebase. These utilities provide common functionality that is used in multiple places, promoting code reuse and reducing redundancy. They are organized in different files under the 'utils' directory, each file focusing on a specific aspect. For example, 'meshReadUtils' contains utilities for reading mesh data, 'jointWriteUtils' for writing joint data, 'writeUtil' for general writing utilities, and so on. These utilities play a crucial role in the functionality of the maya-usd plugin, aiding in tasks such as reading and writing data, handling shading, managing transformations, and more.

<SwmSnippet path="/lib/mayaUsd/fileio/utils/meshReadUtils.cpp" line="93">

---

# Utils in meshReadUtils.cpp

In `meshReadUtils.cpp`, the anonymous namespace contains utility functions such as `addCreaseSet` which is used for adding crease set information to a mesh.

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

In `writeUtil.cpp`, the anonymous namespace contains utility functions such as `_IsAnimated` which checks if a Maya plug is animated.

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

In `jointWriteUtils.h`, the `UsdMayaJointUtil` namespace contains utility functions for dealing with writing out joint and skin data. Functions like `getJointHierarchyComponents` and `getJointNames` are used to retrieve joint hierarchy and joint names respectively.

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

In `meshWriteUtils.h`, the `UsdMayaMeshWriteUtils` namespace contains utility functions for dealing with writing USD from Maya mesh/subdiv tags. Functions like `getSkinClusterConnectedToMesh` and `calcBBoxOfMeshes` are used to retrieve skin cluster connected to a mesh and calculate the bounding box of a given array of meshes respectively.

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

In `writeUtil.h`, the `UsdMayaWriteUtil` struct contains helper functions for writing USD. Functions like `SetAttribute` and `SetUsdAttr` are used to set USD attributes.

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

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
