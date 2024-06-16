---
title: Basic Concepts of Rendering
---
Rendering in the context of the maya-usd repository refers to the process of generating an image from a model. This involves the use of classes and methods that handle the computation and display of geometry, lighting, and shading in the scene. The 'MeshViewportCompute' class, for instance, is used for evaluating geometry streams and OpenSubdiv (OSD). It has methods for setting topology, preparing adjacency buffers, and computing normals and OSD. The '\_Render' method in the 'UsdMayaGLBatchRenderer' class is responsible for executing the render tasks, which includes setting the camera state, saving the current GL states, and executing Hydra tasks. The 'HdVP2RenderPass' class, although not used for rendering in VP2, is required by the HdEngine and HdRenderDelegate. The 'RenderItemData' struct provides storage for render item data, which includes information about the render item, its geometry, and its shader.

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/drawItem.h" line="178">

---

# Rendering Functions

The `SetUsage` function is used to set a usage to the render item. This is part of the process of preparing a render item for rendering.

```c
    /*! \brief  Set a usage to the render item
     */
    void SetUsage(RenderItemUsage usage) { _renderItemUsage = usage; }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/drawItem.h" line="182">

---

The `AddUsage` function is used to add a usage to the render item. This is another part of the process of preparing a render item for rendering.

```c
    /*! \brief  Add a usage to the render item
     */
    void AddUsage(RenderItemUsage usage) { _renderItemUsage |= usage; }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/drawItem.h" line="43">

---

# Rendering Data

The `RenderItemData` class provides storage for render item data. This data is used during the rendering process.

```c
    //! Helper struct providing storage for render item data
    struct RenderItemData
    {
        //! Unique name for easier debugging and profiling.
        MString _renderItemName;
        //! Pointer of the render item for fast access. No ownership is held.
        MHWRender::MRenderItem* _renderItem { nullptr };
        //! If the render item is shared, this will store the shared counter
        std::shared_ptr<size_t> _sharedRenderItemCounter;

        //! The geom subset this render item represents. _geomSubset.id is StdPath::EmptyPath() if
        //! there is no geom subset.
        HdGeomSubset _geomSubset;

        //! Render item index buffer - use when updating data
        std::unique_ptr<MHWRender::MIndexBuffer> _indexBuffer;
        bool                                     _indexBufferValid { false };
        //! Bounding box of the render item.
        MBoundingBox _boundingBox;
        //! World matrix of the render item.
        MMatrix _worldMatrix;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/batchRenderer.cpp" line="1128">

---

# Rendering Operations

The `_Render` method is a key part of the rendering process. It sets up the camera state, prepares render tasks, and executes the Hydra engine to perform the actual rendering.

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

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
