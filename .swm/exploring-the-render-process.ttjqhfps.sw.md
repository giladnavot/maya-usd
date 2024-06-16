---
title: Exploring the Render Process
---

In the context of the maya-usd repository, 'Render' refers to the process of generating an image from a model. This involves a series of computations that transform a scene representation to a final image. The scene representation contains geometric models, lights, shaders, textures, and a camera. The 'Render' process involves several stages, including visibility determination, shading, lighting, and image sampling. The maya-usd repo provides a variety of classes and functions to facilitate the rendering process. For example, the `MeshViewportCompute` class is used to compute geometry streams and OpenSubdiv (OSD) for viewport 2.0, and the `UsdMayaGLBatchRenderer` class is used to manage batched drawing operations for USD data.

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverride.cpp" line="435">

---

# Render Method

This is the 'Render' method in the MtohRenderOverride class. It starts by checking if the render override is in use and then proceeds to execute the rendering tasks. It handles playblasting by executing the render task until the renderer signals convergence. It also manages the state of the frame buffer and executes the rendering engine. If the initialization of the Hydra resources fails, it returns a failure status.

```c++
MStatus MtohRenderOverride::Render(const MHWRender::MDrawContext& drawContext)
{
    // It would be good to clear the resources of the overrides that are
    // not in active use, but I'm not sure if we have a better way than
    // the idle time we use currently. The approach below would break if
    // two render overrides were used at the same time.
    // for (auto* override: _allInstances) {
    //     if (override != this) {
    //         override->ClearHydraResources();
    //     }
    // }
    TF_DEBUG(HDMAYA_RENDEROVERRIDE_RENDER).Msg("MtohRenderOverride::Render()\n");
    auto renderFrame = [&](bool markTime = false) {
        HdTaskSharedPtrVector tasks = _taskController->GetRenderingTasks();

        // For playblasting, a glReadPixels is going to occur sometime after we return.
        // But if we call Execute on all of the tasks, then z-buffer fighting may occur
        // because every colorize/present task is going to be drawing a full-screen quad
        // with 'unconverged' depth.
        //
        // To work arround this (for not Storm) we pull the first task, (render/synch)
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/drawItem.h" line="180">

---

# SetUsage Method

The 'SetUsage' method is used to set a usage to the render item. This is used to specify how the render item is used in the rendering process.

```c
    void SetUsage(RenderItemUsage usage) { _renderItemUsage = usage; }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/drawItem.h" line="184">

---

# AddUsage Method

The 'AddUsage' method is used to add a usage to the render item. This is used when multiple usages are required for a single render item.

```c
    void AddUsage(RenderItemUsage usage) { _renderItemUsage |= usage; }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/meshViewportCompute.h" line="140">

---

# MeshViewportCompute Class

The 'MeshViewportCompute' class is a compute class for evaluating geometry streams and OpenSubdiv (OSD). It handles the computation of the mesh viewport, including the management of instance transforms and colors, shader assignment, and geometry information.

```c
class MeshViewportCompute : public MPxViewportComputeItem
{
private:
    std::shared_ptr<HdVP2MeshSharedData> _meshSharedData;
    bool        _executed { false };   // Has this compute been executed
    bool        _sourcesExecuted {
        false
    }; // Have the source compute's executed. only valid for a consolidated compute

    MSharedPtr<MeshViewportCompute> _consolidatedCompute;

    std::unique_ptr<MHWRender::MGeometryIndexMapping> _geometryIndexMapping;
    unsigned int                                      _vertexCount { 0 };
    GLuint                                            _uboResourceHandle { 0 };

    // adjacency information for normals
    size_t                                    _adjacencyBufferSize { 0 };
    std::unique_ptr<const int>                _adjacencyBufferCPU;
    std::unique_ptr<MHWRender::MVertexBuffer> _adjacencyBufferGPU;
    std::unique_ptr<MHWRender::MVertexBuffer> _renderingToSceneFaceVtxIdsGPU;
    std::unique_ptr<MHWRender::MVertexBuffer> _sceneToRenderingFaceVtxIdsGPU;
```

---

</SwmSnippet>

# Rendering Functions

This section will cover the main functions related to rendering in the Maya-USD plugin.

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverride.cpp" line="185">

---

## MtohRenderOverride

The `MtohRenderOverride` function is a constructor for the MtohRenderOverride class. It initializes the render override with the given renderer description. It sets up various callbacks and initializes resources for Hydra rendering.

```c++
MtohRenderOverride::MtohRenderOverride(const MtohRendererDescription& desc)
    : MHWRender::MRenderOverride(desc.overrideName.GetText())
    , _rendererDesc(desc)
    , _globals(MtohRenderGlobals::GetInstance())
    , _hgi(Hgi::CreatePlatformDefaultHgi())
    , _hgiDriver { HgiTokens->renderDriver, VtValue(_hgi.get()) }
    , _selectionTracker(new HdxSelectionTracker)
    , _isUsingHdSt(desc.rendererName == MtohTokens->HdStormRendererPlugin)
{
    TF_DEBUG(HDMAYA_RENDEROVERRIDE_RESOURCES)
        .Msg(
            "MtohRenderOverride created (%s - %s - %s)\n",
            _rendererDesc.rendererName.GetText(),
            _rendererDesc.overrideName.GetText(),
            _rendererDesc.displayName.GetText());
    HdMayaDelegateRegistry::InstallDelegatesChangedSignal([this]() { _needsClear.store(true); });
    _ID = SdfPath("/HdMayaViewportRenderer")
              .AppendChild(
                  TfToken(TfStringPrintf("_HdMaya_%s_%p", desc.rendererName.GetText(), this)));

    MStatus status;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverride.cpp" line="287">

---

## UpdateRenderGlobals

The `UpdateRenderGlobals` function updates the global render settings. It checks if the attribute affects the renderer and applies the settings accordingly. It also forces an invalidation if the attribute starts with 'mtoh'.

```c++
void MtohRenderOverride::UpdateRenderGlobals(
    const MtohRenderGlobals& globals,
    const TfToken&           attrName)
{
    // If no attribute or attribute starts with 'mtoh', these setting wil be applied on the next
    // call to MtohRenderOverride::Render, so just force an invalidation
    // XXX: This will need to change if mtoh settings should ever make it to the delegate itself.
    if (attrName.GetString().find("mtoh") != 0) {
        std::lock_guard<std::mutex> lock(_allInstancesMutex);
        for (auto* instance : _allInstances) {
            const auto& rendererName = instance->_rendererDesc.rendererName;

            // If no attrName or the attrName is the renderer, then update everything
            const size_t attrFilter = (attrName.IsEmpty() || attrName == rendererName) ? 0 : 1;
            if (attrFilter && !instance->_globals.AffectsRenderer(attrName, rendererName)) {
                continue;
            }

            // Will be applied in _InitHydraResources later anyway
            if (auto* renderDelegate = instance->_GetRenderDelegate()) {
                instance->_globals.ApplySettings(
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/batchRenderer.cpp" line="1032">

---

## \_ComputeSelection

The `_ComputeSelection` function computes the selection of objects in the viewport. It checks the visibility of each object and determines if it is selected or not. It also handles the selection of objects in depth.

```c++
void UsdMayaGLBatchRenderer::_ComputeSelection(
    _ShapeAdapterBucketsMap& bucketsMap,
    const M3dView*           view3d,
    const GfMatrix4d&        viewMatrix,
    const GfMatrix4d&        projectionMatrix,
    const bool               singleSelection)
{
    TRACE_FUNCTION();

    MProfilingScope profilingScope(
        ProfilerCategory, MProfiler::kColorE_L3, "Batch Renderer Computing Selection");

    // If depth selection has not been turned on, then we can optimize
    // area/marquee selections by handling collections similarly to a single
    // selection, where we test intersections against the single, viewport
    // renderer-based collection.
    const bool useDepthSelection = (!singleSelection && _enableDepthSelection);

    const PxrMayaHdPrimFilterVector primFilters
        = _GetIntersectionPrimFilters(bucketsMap, view3d, useDepthSelection);

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/batchRenderer.cpp" line="1128">

---

## \_Render

The `_Render` function is responsible for rendering the scene. It sets up the camera state, prepares the render tasks, and executes them. It also handles the viewport and display style settings.

```c++
void UsdMayaGLBatchRenderer::_Render(
    const GfMatrix4d&               worldToViewMatrix,
    const GfMatrix4d&               projectionMatrix,
    const GfVec4d&                  viewport,
    unsigned int                    displayStyle,
    const std::vector<_RenderItem>& items)
{
    TRACE_FUNCTION();

    MProfilingScope profilingScope(
        ProfilerCategory, MProfiler::kColorC_L2, "Batch Renderer Rendering Batch");

    _taskDelegate->SetCameraState(worldToViewMatrix, projectionMatrix, viewport);

    // save the current GL states which hydra may reset to default
    glPushAttrib(
        GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT | GL_VIEWPORT_BIT);

    GLUniformBufferBindingsSaver bindingsSaver;

    // hydra orients all geometry during topological processing so that
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
