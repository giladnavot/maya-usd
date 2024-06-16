---
title: Overview of USD Scene Description
---

USD Scene Description, often abbreviated as USD, is a framework developed by Pixar for interchange of 3D computer graphics data. It's designed to facilitate the creation, manipulation, and sharing of complex scenes between different tools and applications. In the context of the maya-usd repository, USD is used as the underlying data structure for the 3D scenes. The repository provides a plugin for Maya that allows it to read and write USD files, enabling seamless data exchange with other tools that support USD.

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="91">

---

# Creating a Scene Index

The `_AppendSceneIndex` method is used to create a new scene index. This involves retrieving various data sources from the input arguments, such as the version, node path, and interpret pick function. The method then creates a new `MayaUsdProxyShapeSceneIndex` instance using this data.

```c++
HdSceneIndexBaseRefPtr MayaUsdProxyShapeMayaNodeSceneIndexPlugin::_AppendSceneIndex(
    const HdSceneIndexBaseRefPtr&      inputScene,
    const HdContainerDataSourceHandle& inputArgs)
{
    using HdMObjectDataSource = HdRetainedTypedSampledDataSource<MObject>;
    static TfToken         dataSourceNodePathEntry("object");
    HdDataSourceBaseHandle dataSourceEntryPathHandle = inputArgs->Get(dataSourceNodePathEntry);

    // Retrieve the version integer. The Version integer combines major, minor, and patch number
    // like this major * 10000 + minor * 100 + patch
    using MayaHydraVersionDataSource = HdRetainedTypedSampledDataSource<int>;
    static TfToken         dataSourceVersionEntry("version");
    HdDataSourceBaseHandle dataSourceEntryVersionHandle = inputArgs->Get(dataSourceVersionEntry);
    int version = 100; // initial mayaHydra version where version data source had not been defined
    if (auto dataSourceEntryVersion
        = MayaHydraVersionDataSource::Cast(dataSourceEntryVersionHandle)) {
        version = dataSourceEntryVersion->GetTypedValue(0.0f);
    }

    if (auto dataSourceEntryNodePath = HdMObjectDataSource::Cast(dataSourceEntryPathHandle)) {
        if (version >= 200) {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="356">

---

# Populating the Scene Index

The `Populate` method is used to populate the scene index with data from a USD stage. This involves getting the USD stage from the proxy shape, setting it on the scene index, and then populating the scene index if the stage has children. The method also updates the time of the scene index.

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

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="356">

---

# Using the Scene Index

Once the scene index is populated, it can be used for various operations. For example, the `UpdateTime` method, called at the end of the `Populate` method, is used to update the time of the scene index, which can affect the state of the scene.

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

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
