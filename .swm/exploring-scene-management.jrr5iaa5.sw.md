---
title: Exploring Scene Management
---

Scene Management in maya-usd refers to the process of handling and manipulating the 3D environment or 'scene' in Maya using Pixar's Universal Scene Description (USD). This includes creating, editing, and deleting objects, managing their attributes, and controlling their hierarchical relationships. It also involves managing the USD stages, which are the highest level of organization in a USD file, and the layers that make up these stages. The codebase provides various functions and methods for these operations, such as the `cleanUpNewScene` method for removing all layers from a scene, or the `initialize` function for setting up the stage model.

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="1109">

---

## Scene Cleanup

The `cleanUpNewScene` function is used to remove all layers from the current scene and remove the manager node. This is typically used when a new scene is being created or the current scene is being discarded.

```c++
void LayerDatabase::cleanUpNewScene(void*)
{
    LayerDatabase::instance().removeAllLayers();
    LayerDatabase::removeManagerNode();
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/Global.cpp" line="184">

---

## Scene Initialization

The `initialize` function is used to set up the stage model for the scene. It sets up various functions and handlers required for the plugin to work correctly. This function is typically called during the plugin initialization.

```c++
MStatus initialize()
{
    // If we're already registered, do nothing.
    if (gRegistrationCount++ > 0)
        return MS::kSuccess;

    // Set the Maya specific functions required for the UsdUfe plugin to work correctly.
    UsdUfe::DCCFunctions dccFunctions;
    dccFunctions.stageAccessorFn = mayaGetStage;
    dccFunctions.stagePathAccessorFn = MayaUsd::ufe::stagePath;
    dccFunctions.ufePathToPrimFn = MayaUsd::ufe::ufePathToPrim;
    dccFunctions.timeAccessorFn = MayaUsd::ufe::getTime;
    dccFunctions.saveStageLoadRulesFn = MayaUsd::MayaUsdProxyShapeStageExtraData::saveLoadRules;
    dccFunctions.uniqueChildNameFn = MayaUsd::ufe::uniqueChildNameMayaStandard;
    dccFunctions.displayMessageFn[static_cast<int>(UsdUfe::MessageType::kInfo)]
        = displayInfoMessage;
    dccFunctions.displayMessageFn[static_cast<int>(UsdUfe::MessageType::kWarning)]
        = displayWarningMessage;
    dccFunctions.displayMessageFn[static_cast<int>(UsdUfe::MessageType::KError)]
        = displayErrorMessage;
    dccFunctions.startWaitCursorFn = mayaStartWaitCursor;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/UsdConnections.cpp" line="56">

---

## Scene Connections

The `allConnections` function retrieves all connections in the scene. This includes connections where an attribute of the scene item is the destination. This is useful for understanding the relationships between different elements in the scene.

```c++
std::vector<Ufe::Connection::Ptr> UsdConnections::allConnections() const
{
    if (!TF_VERIFY(_sceneItem))
        return {};

    std::vector<Ufe::Connection::Ptr> result;

    // Get some information of the prim.

    const PXR_NS::UsdPrim prim = _sceneItem->prim();
    const Ufe::Path       primPath = _sceneItem->path();
    const Ufe::Path       stagePath = primPath.getSegments()[0];

    // The method looks for all the connections in which one of the attribute of this scene item is
    // the destination.

    PXR_NS::UsdShadeConnectableAPI connectableAttrs(prim);

    // Helper method to create the in-memory connection if any.
    auto createConnections = [&](const auto& attr) -> void {
        if (attr.HasConnectedSource()) {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="192">

---

## Scene Node Copying

The `isCopyingSceneNodes` function checks if Maya is currently copying nodes. This is typically used to determine if certain operations should be performed, such as saving USD data.

```c++
bool isCopyingSceneNodes()
{
    // When Maya is copy nodes, it exports them and sets this environment
    // variable during the export to let exporters know it is cutting or
    // copying nodes in a temporary Maya scene file.
    return PXR_NS::ArchHasEnv("MAYA_CUT_COPY_EXPORT");
}
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
