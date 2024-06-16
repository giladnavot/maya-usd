---
title: Introduction to Scene Index
---

The Scene Index in the maya-usd repository refers to the structure that organizes and manages the various elements or 'primitives' in a scene. It is a key component of the USD (Universal Scene Description) system, which is used to represent 3D scenes in a hierarchical and efficient manner. The Scene Index is used to store and access these primitives, which can include objects like meshes, lights, materials, and more. It provides methods to add, remove, or query primitives in the scene. The Scene Index is crucial for the rendering process, as it allows the renderer to efficiently traverse the scene and access the necessary data.

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="91">

---

# Scene Index Methods

The '\_AppendSceneIndex' method is used to append data to the Scene Index. It takes in the input scene and input arguments, retrieves the version integer, and based on the version, it retrieves the interpret pick function from the scene index plugin or the UFE runtime id data source. It then creates a new scene index based on the USD version.

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

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="381">

---

The 'GetPrim' method is used to retrieve data from the Scene Index. It checks if the scene index is populated, if not, it populates the scene index. It then returns the prim at the specified path from the input scene index.

```c++
// satisfying HdSceneIndexBase
HdSceneIndexPrim MayaUsdProxyShapeSceneIndex::GetPrim(const SdfPath& primPath) const
{
    // MAYA-126790 TODO: properly resolve missing PrimsAdded notification issue
    // https://github.com/PixarAnimationStudios/USD/blob/dev/pxr/imaging/hd/sceneIndex.cpp#L38
    // Pixar has discussed adding a missing overridable virtual function when an observer is
    // registered For now GetPrim called with magic string populates the scene index
    static SdfPath maya126790Workaround("maya126790Workaround");
    if (primPath == maya126790Workaround) {
        auto nonConstThis = const_cast<MayaUsdProxyShapeSceneIndex*>(this);
        nonConstThis->Populate();
        return HdSceneIndexPrim();
    }

    return _GetInputSceneIndex()->GetPrim(primPath);
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="301">

---

The 'New' method is used to create a new instance of the MayaUsdProxyShapeSceneIndex. It takes in a proxy shape, the last element of the scene index chain, and the UsdImagingStageSceneIndex, and returns a new instance of the MayaUsdProxyShapeSceneIndex.

```c++
MayaUsdProxyShapeSceneIndexRefPtr MayaUsdProxyShapeSceneIndex::New(
    MayaUsdProxyShapeBase*                 proxyShape,
    const HdSceneIndexBaseRefPtr&          sceneIndexChainLastElement,
    const UsdImagingStageSceneIndexRefPtr& usdImagingStageSceneIndex)
{
    // Create the proxy shape scene index which populates the stage
    return TfCreateRefPtr(new MayaUsdProxyShapeSceneIndex(
        sceneIndexChainLastElement, usdImagingStageSceneIndex, proxyShape));
}
```

---

</SwmSnippet>

# Scene Index Endpoints

Scene Index Interactions

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.h" line="53">

---

## \_AppendSceneIndex

The `_AppendSceneIndex` method is used to append a new Scene Index to the existing Scene Index. It takes as input the current Scene Index and the arguments for the new Scene Index.

```c
    HdSceneIndexBaseRefPtr _AppendSceneIndex(
        const HdSceneIndexBaseRefPtr&      inputScene,
        const HdContainerDataSourceHandle& inputArgs) override;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.h" line="85">

---

## GetPrim

The `GetPrim` method is used to retrieve a primitive from the Scene Index using its path. It returns an `HdSceneIndexPrim` object that represents the primitive.

```c
    HdSceneIndexPrim GetPrim(const SdfPath& primPath) const override;
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
