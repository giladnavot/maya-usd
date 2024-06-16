---
title: Introduction to Viewport 2.0 Rendering
---
Viewport 2.0 Rendering in Maya-USD refers to the process of generating images from 3D data within the Viewport 2.0 framework. It utilizes the MeshViewportCompute class, which is responsible for evaluating geometry streams and OpenSubdiv (OSD) requirements. This class executes GPU compute kernels to fulfill the geometry requirements of an HdVP2Mesh. It also allows for the consolidation of MRenderItems from different HdVP2Mesh objects that have the same compute requirements, thereby improving performance by executing their compute in a single consolidated compute kernel.

The rendering process involves various components such as the MGeometry object, which contains the geometry to be rendered, and various buffers like the position, normal, and color vertex buffers. The process also involves the use of adjacency information for normals and geometry information.

The rendering process is also influenced by the display style, which determines how the objects are visually represented in the viewport. This is handled by the GetMFrameContextDisplayStyle function, which converts the display style from the legacy viewport into the display style for Viewport 2.0.

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/meshViewportCompute.h" line="121">

---

## MeshViewportCompute Class

The `MeshViewportCompute` class is a key component in the Viewport 2.0 Rendering process. It is responsible for evaluating geometry streams and OSD requirements for an `HdVP2Mesh`. It also consolidates `MRenderItems` from different `HdVP2Mesh` objects which have the same compute requirements, allowing for efficient execution of compute kernels.

```c
PXR_NAMESPACE_OPEN_SCOPE

struct HdVP2MeshSharedData;
class HdVP2DrawItem;
class PxrMayaGLSLProgram;

/*! \brief  HdVP2Mesh-specific compute class for evaluating geometry streams and OSD
    \class  MeshViewportCompute

    A mesh can have shader stream requires (such as normals, tangents etc) and OSD
    requirements. This class executes GPU compute kernels to fulfill the geometry
    requirements of an HdVP2Mesh.

    A key performance feature of MeshViewportCompute is MRenderItems from different
    HdVP2Mesh objects which have the same compute requirements can be consolidated
    together in consolidated world and have their compute executed in a single
    consolidated compute kernel, rather than issuing a compute kernel per unconsolidated
    render item.
*/
class MeshViewportCompute : public MPxViewportComputeItem
{
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/px_vp20/utilsLegacy.h" line="44">

---

## GetMFrameContextDisplayStyle Function

The `GetMFrameContextDisplayStyle` function is used to convert `M3dView::DisplayStyle` from the legacy viewport into `MHWRender::MFrameContext::DisplayStyle` for Viewport 2.0. This function is essential for translating the display styles from the legacy viewport to the new viewport.

```c
    /// Helper function that converts M3dView::DisplayStyle from the legacy
    /// viewport into MHWRender::MFrameContext::DisplayStyle for Viewport
    /// 2.0.
    ///
    /// In the legacy viewport, the M3dView can be in exactly one
    /// displayStyle whereas Viewport 2.0's displayStyle is a bitmask of
    /// potentially multiple styles. To translate from the legacy viewport
    /// to Viewport 2.0, we simply bitwise-OR the single legacy viewport
    /// displayStyle into an empty mask.
    static unsigned int GetMFrameContextDisplayStyle(M3dView::DisplayStyle legacyDisplayStyle)
    {
        unsigned int displayStyle = 0u;

        switch (legacyDisplayStyle) {
        case M3dView::kBoundingBox:
            displayStyle |= MHWRender::MFrameContext::DisplayStyle::kBoundingBox;
            break;
        case M3dView::kFlatShaded:
            displayStyle |= MHWRender::MFrameContext::DisplayStyle::kFlatShaded;
            break;
        case M3dView::kGouraudShaded:
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/meshViewportCompute.cpp" line="370">

---

## findRenderGeometry Method

The `findRenderGeometry` method is used to find the render geometry for a given `MRenderItem`. This method is crucial for the rendering process as it helps in identifying the geometry to be rendered in the viewport.

```c++
void MeshViewportCompute::findRenderGeometry(MRenderItem& renderItem)
{
    MProfilingScope subProfilingScope(
        HdVP2RenderDelegate::sProfilerCategory,
        MProfiler::kColorD_L2,
        "MeshViewportCompute:findRenderGeometry");

    MGeometry* renderGeometry = renderItem.geometry();
    if (_renderGeom && (_renderGeom != renderGeometry)) {
        _positionVertexBufferGPU = nullptr;
        _normalVertexBufferGPU = nullptr;
        _colorVertexBufferGPU = nullptr;
        _renderGeom = nullptr;
    }
    _renderGeom = renderGeometry;
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
