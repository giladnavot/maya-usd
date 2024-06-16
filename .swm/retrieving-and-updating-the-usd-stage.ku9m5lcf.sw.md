---
title: Retrieving and Updating the USD Stage
---

This document will cover the process of retrieving and updating the USD stage in the MayaUsd plugin, which includes:

1.  Getting the prim
2.  Populating the stage
3.  Updating the time
4.  Getting the time
5.  Getting the proxy shape node
6.  Getting the proxy shape
7.  Rebuilding if dirty
8.  Adding an item
9.  Getting the stage from an object
10. Getting the proxy shape from an object.

## Where is this flow used?

The flow starts with the function `GetPrim`. It is called from multiple entry points as represented in the following diagram: (Note - these are only some of the entry points of this flow)

```mermaid
graph TD;
subgraph lib/mayaUsd/fileio/translators
  update:::rootsStyle --> findExistingMayaReference
end
subgraph lib/mayaUsd/fileio/translators
  findExistingMayaReference --> connectReferenceAssociatedNode
end
subgraph lib/mayaUsd
  connectReferenceAssociatedNode --> connect
end
subgraph lib/mayaUsd
  connect --> connectItems
end
subgraph lib/mayaUsd
  connectItems --> getOrCreateAccessPlug
end
subgraph lib/mayaUsd
  getOrCreateAccessPlug --> getAccessPlug
end
subgraph lib/mayaUsd
  getAccessPlug --> getSdfValueType
end
subgraph lib/mayaUsd
  getSdfValueType --> GetPrim:::mainFlowStyle
end
subgraph lib/mayaUsd/fileio/shading
  ExportShadingEngines:::rootsStyle --> DoExport
end
subgraph lib/mayaUsd/fileio/shading
  DoExport --> Export
end
subgraph lib/mayaUsd/fileio/shading
  Export --> BindStandardMaterialPrim
end
subgraph lib/mayaUsd/fileio/shading
  BindStandardMaterialPrim --> _AssignComponentTags
end
subgraph lib/mayaUsd
  _AssignComponentTags --> GetPrim:::mainFlowStyle
end
subgraph lib/mayaUsd/fileio/shading
  cacheMayaReference:::rootsStyle --> Export
end
subgraph lib/mayaUsd/fileio/shading
  saveLayerWithFormat:::rootsStyle --> Export
end
subgraph lib/mayaUsd/fileio/shading
  Export:::rootsStyle --> BindStandardMaterialPrim
end

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

## The flow itself

```mermaid
graph TD;
subgraph lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp
  GetPrim:::mainFlowStyle --> Populate
end
subgraph lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp
  Populate:::mainFlowStyle --> UpdateTime
end
subgraph lib/mayaUsd/ufe
  UpdateTime:::mainFlowStyle --> getTime
end
subgraph lib/mayaUsd/ufe
  getTime:::mainFlowStyle --> proxyShapeNode
end
subgraph lib/mayaUsd/ufe
  proxyShapeNode:::mainFlowStyle --> proxyShape
end
subgraph lib/mayaUsd/ufe
  proxyShape:::mainFlowStyle --> rebuildIfDirty
end
subgraph lib/mayaUsd/ufe
  rebuildIfDirty:::mainFlowStyle --> addItem
end
subgraph lib/mayaUsd/ufe
  addItem:::mainFlowStyle --> objToStage
end
subgraph lib/mayaUsd/ufe
  objToStage:::mainFlowStyle --> objToProxyShape
end
  objToProxyShape:::mainFlowStyle --> ...

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="1">

---

# Getting the prim

The process starts with the `GetPrim` function. This function is responsible for retrieving the prim from the USD stage.

```c++
//
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="356">

---

# Populating the stage

The `Populate` function is called next. This function checks if the stage has been populated. If not, it retrieves the USD stage from the proxy shape and populates it.

```c++
void MayaUsdProxyShapeSceneIndex::Populate()
{
    if (!_populated) {
        auto stage = _proxyShape->getUsdStage();
        if (TF_VERIFY(stage, "Unable to fetch a valid USDStage.")) {
            _usdImagingStageSceneIndex->SetStage(stage);
            // Check whether the pseudo-root has children
            if (!stage->GetPseudoRoot().GetChildren().empty())
            // MAYA-126641: Upon first call to MayaUsdProxyShapeBase::getUsdStage, the stage may be
            // empty. Do not mark the scene index as _populated, until stage is full. This is taken
            // care of inside MayaUsdProxyShapeSceneIndex::_StageSet callback.
            {
#if PXR_VERSION < 2305
                // In most recent USD, Populate is called from within SetStage, so there is no need
                // to callsites to call it explicitly
                _usdImagingStageSceneIndex->Populate();
#endif
                _populated = true;
            }
            // Set the initial time
            UpdateTime();
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="325">

---

# Updating the time

The `UpdateTime` function is then called. This function updates the time on the USD stage.

```c++
void MayaUsdProxyShapeSceneIndex::UpdateTime()
{
    if (_usdImagingStageSceneIndex && _proxyShape && isProxyShapeValid()) {
        _usdImagingStageSceneIndex->SetTime(_proxyShape->getTime());
    }
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/Utils.cpp" line="282">

---

# Getting the time

The `getTime` function is called to retrieve the current time from the proxy shape node.

```c++
UsdTimeCode getTime(const Ufe::Path& path)
{
    // Path should not be empty.
    if (!TF_VERIFY(!path.empty())) {
        return UsdTimeCode::Default();
    }

    // Proxy shape node should not be null.
    auto proxyShape = UsdStageMap::getInstance().proxyShapeNode(path);
    if (!TF_VERIFY(proxyShape)) {
        return UsdTimeCode::Default();
    }

    return proxyShape->getTime();
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/UsdStageMap.cpp" line="235">

---

# Getting the proxy shape node

The `proxyShapeNode` function is called to retrieve the proxy shape node from the path.

```c++
MayaUsdProxyShapeBase* UsdStageMap::proxyShapeNode(const Ufe::Path& path, bool rebuildCacheIfNeeded)
{
    auto obj = proxyShape(path, rebuildCacheIfNeeded);
    if (obj.isNull()) {
        return nullptr;
    }

    return objToProxyShape(obj);
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/UsdStageMap.cpp" line="170">

---

# Getting the proxy shape

The `proxyShape` function is called to retrieve the proxy shape from the path.

```c++
MObject UsdStageMap::proxyShape(const Ufe::Path& path, bool rebuildCacheIfNeeded)
{
    // Optimization: if there are not proxy shape instances,
    // there is nothing that can be mapped.
    if (MayaUsdProxyShapeBase::countProxyShapeInstances() == 0)
        return MObject();

    const bool wasRebuilt = rebuildIfDirty();

    const auto& singleSegmentPath
        = path.nbSegments() == 1 ? path : Ufe::Path(path.getSegments()[0]);

    auto iter = _pathToObject.find(singleSegmentPath);

    if (rebuildCacheIfNeeded && !wasRebuilt) {
        if (iter == std::end(_pathToObject)) {
            for (const auto& psn : ProxyShapeHandler::getAllNames()) {
                auto psPath = toPath(psn);
                if (_pathToObject.find(psPath) == std::end(_pathToObject)) {
                    addItem(psPath);
                }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/UsdStageMap.cpp" line="286">

---

# Rebuilding if dirty

The `rebuildIfDirty` function is called to rebuild the stage map if it is marked as dirty.

```c++
bool UsdStageMap::rebuildIfDirty()
{
    if (!_dirty)
        return false;

    for (const auto& psn : ProxyShapeHandler::getAllNames()) {
        addItem(toPath(psn));
    }

    TF_DEBUG(MAYAUSD_STAGEMAP)
        .Msg("Rebuilt stage map, found %d proxy shapes\n", int(_stageToObject.size()));
    _dirty = false;
    return true;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/UsdStageMap.cpp" line="133">

---

# Adding an item

The `addItem` function is called to add an item to the stage map.

```c++
void UsdStageMap::addItem(const Ufe::Path& path)
{
    // We expect a path to the proxy shape node, therefore a single segment.
    auto nbSegments = path.nbSegments();
    if (nbSegments != 1) {
        TF_CODING_ERROR(
            "A proxy shape node path can have only one segment, path '%s' has %lu",
            path.string().c_str(),
            nbSegments);
        return;
    }

    // Convert the UFE path to an MObjectHandle.
    auto proxyShape = nameLookup(path);
    if (!proxyShape.isValid()) {
        return;
    }

    // If a proxy shape doesn't yet have a stage, don't add it.
    // We will add it later, when the stage is initialized
    auto obj = proxyShape.object();
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/UsdStageMap.cpp" line="76">

---

# Getting the stage from an object

The `objToStage` function is called to retrieve the USD stage from an object.

```c++
UsdStageWeakPtr objToStage(MObject& obj)
{
    MayaUsdProxyShapeBase* ps = objToProxyShape(obj);
    if (!ps)
        return {};

    return ps->getUsdStage();
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/UsdStageMap.cpp" line="65">

---

# Getting the proxy shape from an object

Finally, the `objToProxyShape` function is called to retrieve the proxy shape from an object.

```c++
MayaUsdProxyShapeBase* objToProxyShape(MObject& obj)
{
    if (obj.isNull()) {
        return nullptr;
    }

    // Get the stage from the proxy shape.
    MFnDependencyNode fn(obj);
    return dynamic_cast<MayaUsdProxyShapeBase*>(fn.userNode());
}
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
