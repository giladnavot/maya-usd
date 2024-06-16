---
title: Understanding USD Libraries Usage
---

USD Libraries in the context of the maya-usd repository are used to facilitate the creation of a Maya plugin for Pixar's Universal Scene Description (USD). They provide a set of reusable components that are used throughout the codebase. For instance, the `MayaUsdProxyShapeMayaNodeSceneIndexPlugin::_AppendSceneIndex` method in `lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp` uses these libraries to append a scene index. The USD Libraries are essential for the functionality of the maya-usd plugin, as they provide the necessary tools and functions to interact with USD.

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="48">

---

# Including USD Libraries

Here, different USD Libraries are included based on the version of the USD API. For example, `pxr/usdImaging/usdImaging/drawModeSceneIndex.h` is included for USD version 2302 and later, while `pxr/usdImaging/usdImagingGL/drawModeSceneIndex.h` is included for USD version 2211.

```c++
#include <pxr/usdImaging/usdImaging/drawModeSceneIndex.h> //For USD 2302 and later
#include <pxr/usdImaging/usdImaging/niPrototypePropagatingSceneIndex.h>
#include <pxr/usdImaging/usdImaging/piPrototypePropagatingSceneIndex.h>
#else
#include <pxr/imaging/hd/instancedBySceneIndex.h>
#include <pxr/usdImaging/usdImagingGL/drawModeSceneIndex.h> //For USD 2211
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.cpp" line="29">

---

# Checking USD API Version

The version of the USD API is checked using the `USD_IMAGING_API_VERSION` macro. This is used to ensure that the correct libraries are included based on the version of the USD API.

```c++
#if USD_IMAGING_API_VERSION >= 20 // For USD 23.11+
```

---

</SwmSnippet>

# USD Libraries Usage

Understanding USD Libraries Usage

<SwmSnippet path="/lib/mayaUsd/sceneIndex/proxyShapeSceneIndexPlugin.h" line="53">

---

## \_AppendSceneIndex

The `_AppendSceneIndex` function is an endpoint in the `MayaUsdProxyShapeMayaNodeSceneIndexPlugin` class. It is used to append a new scene index to the existing scene.

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

The `GetPrim` function is an endpoint in the `MayaUsdProxyShapeSceneIndex` class. It is used to get a primitive from the scene index by its path.

```c
    HdSceneIndexPrim GetPrim(const SdfPath& primPath) const override;
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
