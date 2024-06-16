---
title: Prepare for Draw Process
---
This document will cover the process of preparing for draw in the Maya USD plugin, which includes:

1. Syncing Shape Adapters
2. Adding Shape Adapter
3. Syncing Instancer Shape Adapter
4. Getting Repr Selector For Display Style
5. Checking Active Display Status.

```mermaid
graph TD;
subgraph lib/mayaUsd/render/pxrUsdMayaGL
  prepareForDraw:::mainFlowStyle --> SyncShapeAdapters
end
subgraph lib/mayaUsd/render/pxrUsdMayaGL
  SyncShapeAdapters:::mainFlowStyle --> _SyncShapeAdapters
end
subgraph lib/mayaUsd/render/pxrUsdMayaGL
  _SyncShapeAdapters:::mainFlowStyle --> AddShapeAdapter
end
subgraph lib/mayaUsd/render/pxrUsdMayaGL
  _SyncShapeAdapters:::mainFlowStyle --> Sync
end
subgraph lib/mayaUsd/render/pxrUsdMayaGL
  Sync:::mainFlowStyle --> _Sync
end
subgraph lib/mayaUsd/render/pxrUsdMayaGL
  _Sync:::mainFlowStyle --> GetReprSelectorForDisplayStyle
end
subgraph lib/mayaUsd/render/pxrUsdMayaGL
  GetReprSelectorForDisplayStyle:::mainFlowStyle --> _IsActiveDisplayStatus
end
  _IsActiveDisplayStatus:::mainFlowStyle --> ...

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/instancerImager.cpp" line="49">

---

# Syncing Shape Adapters

The function `SyncShapeAdapters` is used to sync the shape adapters for Viewport 2.0.

```c++
void UsdMayaGL_InstancerImager::SyncShapeAdapters(const unsigned int displayStyle)
{
    // Viewport 2.0 sync.
    _SyncShapeAdapters(true, displayStyle, M3dView::DisplayStyle::kBoundingBox);
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/batchRenderer.cpp" line="122">

---

# Adding Shape Adapter

The function `AddShapeAdapter` is used to add a shape adapter to the batch renderer. It verifies the shape adapter, adds it to the correct bucket based on its render parameters, and adds it to the secondary object handle map.

```c++
bool UsdMayaGLBatchRenderer::AddShapeAdapter(PxrMayaHdShapeAdapter* shapeAdapter)
{
    TRACE_FUNCTION();

    MProfilingScope profilingScope(
        ProfilerCategory, MProfiler::kColorE_L3, "Batch Renderer Adding Shape Adapter");

    if (!TF_VERIFY(shapeAdapter, "Cannot add invalid shape adapter")) {
        return false;
    }

    const bool isViewport2 = shapeAdapter->IsViewport2();

    // Add the shape adapter to the correct bucket based on its renderParams.
    _ShapeAdapterBucketsMap& bucketsMap
        = isViewport2 ? _shapeAdapterBuckets : _legacyShapeAdapterBuckets;

    const PxrMayaHdRenderParams& renderParams = shapeAdapter->GetRenderParams();
    const size_t                 renderParamsHash = renderParams.Hash();

    TF_DEBUG(PXRUSDMAYAGL_SHAPE_ADAPTER_BUCKETING)
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/instancerShapeAdapter.cpp" line="202">

---

# Syncing Instancer Shape Adapter

The function `_Sync` is used to sync the instancer shape adapter. It checks for updates to the shape or changes in the batch renderer that require re-initialization of the shape adapter.

```c++
/* virtual */
bool UsdMayaGL_InstancerShapeAdapter::_Sync(
    const MDagPath&    shapeDagPath,
    const unsigned int displayStyle,
    const MHWRender::DisplayStatus /* displayStatus */)
{
    MStatus               status;
    UsdPrim               usdPrim = _instancerStage->GetDefaultPrim();
    UsdGeomPointInstancer instancer(usdPrim);
    _SyncInstancer(instancer, shapeDagPath);

    // Check for updates to the shape or changes in the batch renderer that
    // require us to re-initialize the shape adapter.
    HdRenderIndex* renderIndex = UsdMayaGLBatchRenderer::GetInstance().GetRenderIndex();
    if (!(shapeDagPath == GetDagPath()) || !_delegate
        || renderIndex != &_delegate->GetRenderIndex()) {
        _SetDagPath(shapeDagPath);

        if (!_Init(renderIndex)) {
            return false;
        }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/shapeAdapter.cpp" line="195">

---

# Getting Repr Selector For Display Style

The function `GetReprSelectorForDisplayStyle` is used to get the representation selector for the given display style. It checks the display style and returns the appropriate representation selector.

```c++
HdReprSelector
PxrMayaHdShapeAdapter::GetReprSelectorForDisplayStyle(unsigned int displayStyle) const
{
    HdReprSelector reprSelector;

    const bool boundingBoxStyle
        = displayStyle & MHWRender::MFrameContext::DisplayStyle::kBoundingBox;

    if (boundingBoxStyle) {
        // We don't currently use Hydra to draw bounding boxes, so we return an
        // empty repr selector here. Also, Maya seems to ignore most other
        // DisplayStyle bits when the viewport is in the kBoundingBox display
        // style anyway, and it just changes the color of the bounding box on
        // selection rather than adding in the wireframe like it does for
        // shaded display styles. So if we eventually do end up using Hydra for
        // bounding boxes, we could just return the appropriate repr here.
        return reprSelector;
    }

    const MHWRender::DisplayStatus displayStatus
        = MHWRender::MGeometryUtilities::displayStatus(_shapeDagPath);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/shapeAdapter.cpp" line="85">

---

# Checking Active Display Status

The function `_IsActiveDisplayStatus` is used to check if the display status is active. It returns true if the display status is active, hilite, active template, active component, or lead.

```c++
static inline bool _IsActiveDisplayStatus(MHWRender::DisplayStatus displayStatus)
{
    return (displayStatus == MHWRender::DisplayStatus::kActive)
        || (displayStatus == MHWRender::DisplayStatus::kHilite)
        || (displayStatus == MHWRender::DisplayStatus::kActiveTemplate)
        || (displayStatus == MHWRender::DisplayStatus::kActiveComponent)
        || (displayStatus == MHWRender::DisplayStatus::kLead);
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="flows"><sup>Powered by [Swimm](/)</sup></SwmMeta>
