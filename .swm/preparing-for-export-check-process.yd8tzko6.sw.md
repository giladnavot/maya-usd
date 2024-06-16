---
title: Preparing for Export Check Process
---

This document will cover the process of preparing for export check in the Maya-USD plugin, which includes:

1. Preparing for write check
2. Removing the manager node
3. Saving USD to USD files
4. Saving USD to Maya files
5. Finding or creating a node
6. Executing the export command
7. Getting the exporter
8. Converting anonymous layers
9. Clearing the manager node.

```mermaid
graph TD;
subgraph lib/mayaUsd
  prepareForExportCheck:::mainFlowStyle --> prepareForWriteCheck
end
subgraph lib/mayaUsd
  prepareForWriteCheck:::mainFlowStyle --> removeManagerNode
end
subgraph lib/mayaUsd
  prepareForWriteCheck:::mainFlowStyle --> saveUsd
end
subgraph lib/mayaUsd
  removeManagerNode --> clearManagerNode
end
subgraph lib/mayaUsd
  clearManagerNode --> disconnectCompoundArrayPlug
end
subgraph lib/mayaUsd
  saveUsd:::mainFlowStyle --> saveUsdToUsdFiles
end
subgraph lib/mayaUsd
  saveUsd:::mainFlowStyle --> saveUsdToMayaFile
end
subgraph lib/mayaUsd
  saveUsdToUsdFiles --> convertAnonymousLayers
end
subgraph lib/mayaUsd
  convertAnonymousLayers --> saveUsdLayerToMayaFile
end
subgraph lib/mayaUsd
  saveUsdToMayaFile:::mainFlowStyle --> findOrCreateNode
end
subgraph lib/mayaUsd
  findOrCreateNode:::mainFlowStyle --> doIt
end
subgraph lib/mayaUsd/fileio/shading
  doIt:::mainFlowStyle --> GetExporter
end
subgraph lib/mayaUsd/fileio/shading
  GetExporter:::mainFlowStyle --> _GetExporter
end
subgraph lib/mayaUsd/fileio/primUpdaterManager.cpp
  _GetExporter:::mainFlowStyle --> end
end
  end:::mainFlowStyle --> ...

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="411">

---

# Preparing for write check

The function `prepareForWriteCheck` is used to prepare for the write check. It cleans up the new scene and saves the layer manager selected stage. If there are any proxies to save, it executes the save command and sets the return code accordingly.

```c++
void LayerDatabase::prepareForWriteCheck(bool* retCode, bool isExport)
{
    _isSavingMayaFile = true;
    cleanUpNewScene(nullptr);

    LayerDatabase::instance().saveLayerManagerSelectedStage();

    bool hasAnyProxy = false;
    if (LayerDatabase::instance().getProxiesToSave(isExport, &hasAnyProxy)) {

        int dialogResult = true;

        if (!isCopyingSceneNodes()) {
            if (MGlobal::kInteractive == MGlobal::mayaState() && !isCrashing()
                && LayerDatabase::instance().saveInteractionRequired()) {
                MGlobal::executeCommand(kSaveOptionUICmd, dialogResult);
            }
        }

        if (dialogResult) {
            dialogResult = LayerDatabase::instance().saveUsd(isExport);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="1221">

---

# Removing the manager node

The function `removeManagerNode` is used to remove the manager node. If the manager node is not provided, it finds the node. It then clears the manager node and deletes it.

```c++
void LayerDatabase::removeManagerNode(MayaUsd::LayerManager* lm)
{
    if (!lm) {
        lm = findNode();
    }
    if (!lm) {
        return;
    }

    // This is called during a Maya notification callback, so no undo supported.
    OpUndoItemMuting muting;

    clearManagerNode(lm);

    MDGModifier& modifier = MDGModifierUndoItem::create("Manager node removal");
    modifier.deleteNode(lm->thisMObject());
    modifier.doIt();
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="875">

---

# Saving USD to USD files

The function `saveUsdToUsdFiles` is used to save USD to USD files. It iterates over the proxies to save and saves the stage to a Maya file if the proxy is not shareable. If the proxy is shareable and not from external sources, it converts anonymous layers and saves the layer if it has permission to save.

```c++
BatchSaveResult LayerDatabase::saveUsdToUsdFiles()
{
    MFnDependencyNode fn;
    for (size_t i = 0; i < _proxiesToSave.size() + _internalProxiesToSave.size(); i++) {
        const StageSavingInfo& info = i < _proxiesToSave.size()
            ? _proxiesToSave[i]
            : _internalProxiesToSave[i - _proxiesToSave.size()];

        MObject mobj = info.dagPath.node();
        fn.setObject(mobj);
        if (!fn.isFromReferencedFile()
            && LayerDatabase::instance().supportedNodeType(fn.typeId())) {
            MayaUsdProxyShapeBase* pShape = static_cast<MayaUsdProxyShapeBase*>(fn.userNode());

            // Unshared Composition Saves to MayaFile Always
            if (!info.shareable) {
                saveStageToMayaFile(mobj, info.stage);
            } else {
                // No need to save stages from external sources
                if (info.isIncoming) {
                    continue;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="964">

---

# Saving USD to Maya files

The function `saveUsdLayerToMayaFile` is used to save USD layer to a Maya file. It finds or creates a node and sets the value for the attribute of the proxy node.

```c++
void LayerDatabase::saveUsdLayerToMayaFile(SdfLayerRefPtr layer, bool asAnonymous)
{
    MayaUsd::LayerManager* lm = findOrCreateNode();
    if (!lm)
        return;

    MStatus           status;
    MDataBlock        dataBlock = lm->_forceCache();
    MArrayDataHandle  layersHandle = dataBlock.outputArrayValue(lm->layers, &status);
    MArrayDataBuilder builder(&dataBlock, lm->layers, 1 /*maybe nb stages?*/, &status);

    addLayerToBuilder(lm, builder, layer, asAnonymous);

    layersHandle.set(builder);

    layersHandle.setAllClean();
    dataBlock.setClean(lm->layers);
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="141">

---

# Finding or creating a node

The function `findOrCreateNode` is used to find or create a node. If the node is not found, it creates a new node.

```c++
MayaUsd::LayerManager* findOrCreateNode()
{
    MayaUsd::LayerManager* lm = findNode();
    if (!lm) {
        MDGModifier& modifier = MayaUsd::MDGModifierUndoItem::create("Node find or creation");
        MObject      manager = modifier.createNode(MayaUsd::LayerManager::typeId);
        modifier.doIt();

        lm = static_cast<MayaUsd::LayerManager*>(MFnDependencyNode(manager).userNode());
    }

    return lm;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/baseExportCommand.cpp" line="288">

---

# Executing the export command

The function `doIt` is used to execute the export command. It checks all flags and reads all of the dictionary args. It then reads all of the other args that are specific to this command. It also validates the paths and gets the filtered selection to export.

```c++
MStatus MayaUSDExportCommand::doIt(const MArgList& args)
{
    try {
        MStatus status;

        MArgDatabase argData(syntax(), args, &status);

        // Check that all flags were valid
        if (status != MS::kSuccess) {
            return status;
        }

        if (argData.isFlagSet("shadingMode")) {
            MString stringVal;
            argData.getFlagArgument("shadingMode", 0, stringVal);
            TfToken shadingMode(stringVal.asChar());

            if (!shadingMode.IsEmpty()
                && UsdMayaShadingModeRegistry::GetInstance().GetExporter(shadingMode) == nullptr
                && shadingMode != UsdMayaShadingModeTokens->none) {
                MGlobal::displayError(
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeRegistry.h" line="75">

---

# Getting the exporter

The function `GetExporter` is used to get the exporter. It calls the `_GetExporter` function to get the exporter.

```c
    static UsdMayaShadingModeExporterCreator GetExporter(const TfToken& name)
    {
        return GetInstance()._GetExporter(name);
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="918">

---

# Converting anonymous layers

The function `convertAnonymousLayers` is used to convert anonymous layers. It converts anonymous layers recursively and saves the USD layer to a Maya file.

```c++
void LayerDatabase::convertAnonymousLayers(
    MayaUsdProxyShapeBase* pShape,
    const MObject&         proxyNode,
    UsdStageRefPtr         stage)
{
    SdfLayerHandle root = stage->GetRootLayer();
    std::string    proxyName(pShape->name().asChar());

    convertAnonymousLayersRecursive(root, proxyName, stage);

    // Note: retrieve root again since it may have been changed by the call
    //       to convertAnonymousLayersRecursive
    root = stage->GetRootLayer();
    if (root->IsAnonymous()) {
        const bool wasTargetLayer = (stage->GetEditTarget().GetLayer() == root);
        PXR_NS::SdfFileFormat::FileFormatArguments args;
        std::string newFileName = MayaUsd::utils::generateUniqueFileName(proxyName);
        if (UsdMayaUtilFileSystem::requireUsdPathsRelativeToMayaSceneFile()) {
            newFileName = UsdMayaUtilFileSystem::getPathRelativeToMayaSceneFile(newFileName);
        }
        if (!MayaUsd::utils::saveLayerWithFormat(root, newFileName)) {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="1200">

---

# Clearing the manager node

The function `clearManagerNode` is used to clear the manager node. It disconnects any connected attributes and wipes the array attribute.

```c++
void LayerDatabase::clearManagerNode(MayaUsd::LayerManager* lm)
{
    if (!lm)
        return;

    MStatus status;
    MPlug   arrayPlug(lm->thisMObject(), lm->layers);

    // First, disconnect any connected attributes
    disconnectCompoundArrayPlug(arrayPlug);

    // Then wipe the array attribute
    MDataBlock       dataBlock = lm->_forceCache();
    MArrayDataHandle layersArrayHandle = dataBlock.outputArrayValue(lm->layers, &status);

    MArrayDataBuilder builder(&dataBlock, lm->layers, 0, &status);
    layersArrayHandle.set(builder);
    layersArrayHandle.setAllClean();
    dataBlock.setClean(lm->layers);
}
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
