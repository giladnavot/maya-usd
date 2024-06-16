---
title: Understanding USD Editing
---
USD Editing in the maya-usd repository refers to the process of making changes to the Universal Scene Description (USD) data within the Maya environment. This can involve operations such as translating, rotating, or scaling objects, renaming prims, or modifying attributes.&nbsp;

The changes are tracked and can be undone or redone, providing a robust system for non-destructive editing. The editing operations can be performed on different layers of the USD stage, and the changes can be saved back to the USD files or to the Maya scene file.&nbsp;

The repository provides various utilities and commands to facilitate USD editing, such as the `UsdTranslateUndoableCommand` for translating objects, or the `doUsdRename` function for renaming prims.

<SwmSnippet path="/lib/mayaUsd/ufe/UsdUndoRenameCommand.cpp" line="149">

---

## Renaming USD Prims

The `doUsdRename` function is used to rename USD prims. It takes in the USD stage, the prim to be renamed, the new name, and the source and destination paths. It updates the internal references paths and sets the new name in the target layer and all other applicable layers.

```c++
void doUsdRename(
    const UsdStagePtr& stage,
    const UsdPrim&     prim,
    std::string        newName,
    const Ufe::Path    srcPath,
    const Ufe::Path    dstPath)
{
    UsdUfe::enforceMutedLayer(prim, "rename");

    // 1- open a changeblock to delay sending notifications.
    // 2- update the Internal References paths (if any) first
    // 3- set the new name
    // Note: during the changeBlock scope we are still working with old items/paths/prims.
    // it's only after the scope ends that we start working with new items/paths/prims
    SdfChangeBlock changeBlock;

    if (!UsdUfe::updateReferencedPath(prim, SdfPath(dstPath.getSegments()[1].string()))) {
        const std::string error = TfStringPrintf(
            "Failed to update references to prim \"%s\".", prim.GetPath().GetText());
        TF_WARN("%s", error.c_str());
        throw std::runtime_error(error);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/UsdTranslateUndoableCommand.cpp" line="27">

---

## Translating Objects

The `UsdTranslateUndoableCommand` method is used to translate objects. It takes in the path of the object and the x, y, and z coordinates for the translation. This command is undoable, meaning that the translation can be reversed if needed.

```c++
UsdTranslateUndoableCommand::UsdTranslateUndoableCommand(
    const Ufe::Path& path,
    double           x,
    double           y,
    double           z)
    : Ufe::TranslateUndoableCommand(path)
    , UsdTRSUndoableCommandBase(x, y, z)
{
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/proxyRenderDelegate.cpp" line="1692">

---

## Adding Display Layers

The `AddDisplayLayerToCache` method is used to add display layers to the cache. It takes in a display layer object and adds it to the cache if it is not the default layer. It also updates the USD paths associated with the display layer.

```c++
void ProxyRenderDelegate::AddDisplayLayerToCache(MObject& displayLayerObj)
{
    if (!displayLayerObj.hasFn(MFn::kDisplayLayer)) {
        return;
    }

    MFnDisplayLayer displayLayer(displayLayerObj);
    if (displayLayer.name() == "defaultLayer") {
        return;
    }

    MSelectionList members;
    displayLayer.getMembers(members);
    auto      membersCount = members.length();
    Ufe::Path proxyShapePath = MayaUsd::ufe::stagePath(_proxyShapeData->UsdStage());
    for (unsigned int j = 0; j < membersCount; ++j) {
        // skip maya paths, as they will be updated from UpdateProxyShapeDisplayLayers
        MDagPath dagPath;
        if (members.getDagPath(j, dagPath) == MS::kSuccess) {
            continue;
        }
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](https://staging.swimm.cloud/)</sup></SwmMeta>
