---
title: Introduction to Render Delegate
---
Render Delegate in the context of the maya-usd repository refers to a mechanism that allows the USD's Hydra rendering architecture to interface with different rendering backends. In this case, the Render Delegate interfaces with Maya's Viewport 2.0 (VP2). The Render Delegate is responsible for translating Hydra/USD concepts such as draw items and changes to scene data into corresponding VP2 concepts. It also manages resources like shaders and textures required for rendering in VP2. The Render Delegate is implemented in the HdVP2RenderDelegate class, which is part of the vp2RenderDelegate directory.

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/renderDelegate.h" line="70">

---

# HdVP2RenderDelegate Class

The `HdVP2RenderDelegate` class is the main implementation of the Render Delegate within the Maya-USD plugin. It inherits from the `HdRenderDelegate` class provided by the USD Hydra architecture. This class contains methods for creating and managing various types of renderable entities (Rprims, Sprims, Bprims), as well as methods for managing resources and committing changes to the render index.

```c
*/
class HdVP2RenderDelegate final : public HdRenderDelegate
{
public:
    HdVP2RenderDelegate(ProxyRenderDelegate& proxyDraw);

    ~HdVP2RenderDelegate() override;

    HdRenderParam* GetRenderParam() const override;

    const TfTokenVector& GetSupportedRprimTypes() const override;

    const TfTokenVector& GetSupportedSprimTypes() const override;

    const TfTokenVector& GetSupportedBprimTypes() const override;

    HdResourceRegistrySharedPtr GetResourceRegistry() const override;

    HdVP2ResourceRegistry& GetVP2ResourceRegistry();

    HdRenderPassSharedPtr
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/renderDelegate.cpp" line="174">

---

# Initialize Function

The `Initialize` function is responsible for initializing the shaders used by the Render Delegate. It is called during the creation of the Render Delegate and ensures that all necessary resources are set up for rendering.

```c++
    /*! \brief  Initialize shaders.
     */
    void Initialize()
    {
        if (_isInitialized)
            return;

        MHWRender::MRenderer*            renderer = MHWRender::MRenderer::theRenderer();
        const MHWRender::MShaderManager* shaderMgr
            = renderer ? renderer->getShaderManager() : nullptr;
        if (!TF_VERIFY(shaderMgr))
            return;

        _3dDefaultMaterialShader
            = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dDefaultMaterialShader);

        TF_VERIFY(_3dDefaultMaterialShader);

        _3dCPVSolidShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dCPVSolidShader);
        if (TF_VERIFY(_3dCPVSolidShader)) {
            _3dCPVSolidShader->setParameter(kDiffuseParameterName, 1.0f);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/renderDelegate.h" line="38">

---

# ProxyRenderDelegate Class

The `ProxyRenderDelegate` class is a helper class used by the `HdVP2RenderDelegate`. It is used to manage the interaction between the Render Delegate and Maya's VP2.

```c
class ProxyRenderDelegate;

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/proxyRenderDelegate.cpp" line="661">

---

# \_InitRenderDelegate Method

The `_InitRenderDelegate` method is responsible for initializing the Render Delegate. It is called when the `ProxyRenderDelegate` is updated. This method ensures that the Render Delegate and the Render Index are properly set up and ready for rendering.

```c++
//! \brief  Initialize the render delegate
void ProxyRenderDelegate::_InitRenderDelegate()
{
    TF_VERIFY(_proxyShapeData->ProxyShape());

    // Initialize the optionVar ShowDisplayColorTextureOff, which will decide if display color will
    // be used when untextured mode is selected
    const MString optionVarName(MayaUsdOptionVars->ShowDisplayColorTextureOff.GetText());
    if (!MGlobal::optionVarExists(optionVarName)) {
        MGlobal::setOptionVarValue(optionVarName, 0);
    }

    // No need to run all the checks if we got till the end
    if (_isInitialized())
        return;

    _proxyShapeData->UpdateUsdStage();
    _proxyShapeData->UsdStageUpdated();

    if (!_renderDelegate) {
        MProfilingScope subProfilingScope(
```

---

</SwmSnippet>

# Render Delegate Functions

The Render Delegate in Maya-USD plugin is responsible for rendering USD data in Maya's viewport. It contains several important functions that handle different aspects of the rendering process.

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/renderDelegate.cpp" line="174">

---

## Initialize

The `Initialize` function is responsible for initializing shaders. It checks if the shaders are already initialized, and if not, it retrieves the shader manager from the renderer and verifies its existence. Then, it gets the stock shaders and sets their parameters. It also initializes the fallback shaders and sets their parameters.

```c++
    /*! \brief  Initialize shaders.
     */
    void Initialize()
    {
        if (_isInitialized)
            return;

        MHWRender::MRenderer*            renderer = MHWRender::MRenderer::theRenderer();
        const MHWRender::MShaderManager* shaderMgr
            = renderer ? renderer->getShaderManager() : nullptr;
        if (!TF_VERIFY(shaderMgr))
            return;

        _3dDefaultMaterialShader
            = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dDefaultMaterialShader);

        TF_VERIFY(_3dDefaultMaterialShader);

        _3dCPVSolidShader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dCPVSolidShader);
        if (TF_VERIFY(_3dCPVSolidShader)) {
            _3dCPVSolidShader->setParameter(kDiffuseParameterName, 1.0f);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/renderDelegate.cpp" line="693">

---

## CreateRenderPass

The `CreateRenderPass` function creates a render pass for rendering a given collection. A render pass in Hydra is a unit of work that encapsulates the data required to draw a set of prims (geometric entities).

```c++
HdRenderPassSharedPtr
HdVP2RenderDelegate::CreateRenderPass(HdRenderIndex* index, const HdRprimCollection& collection)
{
    return HdRenderPassSharedPtr(new HdVP2RenderPass(this, index, collection));
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/renderDelegate.cpp" line="734">

---

## CreateRprim

The `CreateRprim` function creates a new Rprim (render primitive) of a specific type. Rprims are the basic drawable entities in Hydra. This function supports the creation of mesh, basisCurves, and points primitives.

```c++
HdRprim* HdVP2RenderDelegate::CreateRprim(
    const TfToken& typeId,
#if defined(HD_API_VERSION) && HD_API_VERSION >= 36
    const SdfPath& rprimId)
#else
    const SdfPath& rprimId,
    const SdfPath& instancerId)
#endif
{
    if (typeId == HdPrimTypeTokens->mesh) {
#if defined(HD_API_VERSION) && HD_API_VERSION >= 36
        return new HdVP2Mesh(this, rprimId);
#else
        return new HdVP2Mesh(this, rprimId, instancerId);
#endif
    }
    if (typeId == HdPrimTypeTokens->basisCurves) {
#if defined(HD_API_VERSION) && HD_API_VERSION >= 36
        return new HdVP2BasisCurves(this, rprimId);
#else
        return new HdVP2BasisCurves(this, rprimId, instancerId);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/renderDelegate.cpp" line="624">

---

## GetRenderParam

The `GetRenderParam` function returns the render parameter associated with the Render Delegate. The render parameter provides access to the VP2 (Viewport 2.0) during the synchronization of the primitive.

```c++
/*! \brief  Return delegate's HdVP2RenderParam, giving access to things like MPxSubSceneOverride.
 */
HdRenderParam* HdVP2RenderDelegate::GetRenderParam() const { return _renderParam.get(); }
```

---

</SwmSnippet>

# Render Delegate Endpoints

Understanding Render Delegate Endpoints

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/proxyRenderDelegate.h" line="96">

---

## ProxyRenderDelegate

The `ProxyRenderDelegate` class is a key component in the rendering process. It is responsible for the synchronization of data between the scene delegate and VP2, and the final rendering is done by VP2 as part of the subscene override mechanism. It provides several public methods that can be considered as endpoints, such as `supportedDrawAPIs`, `enableUpdateForSelection`, `requiresUpdate`, and `update`.

```c
/*! \brief  USD Proxy rendering routine via VP2 MPxSubSceneOverride

    This drawing routine leverages HdVP2RenderDelegate for synchronization
    of data between scene delegate and VP2. Final rendering is done by VP2
    as part of subscene override mechanism.

    USD Proxy can be rendered in a number of ways and by default we use VP2RenderDelegate.
    Use MAYAUSD_DISABLE_VP2_RENDER_DELEGATE  env variable before loading USD
    plugin to switch to the legacy rendering with draw override approach.
*/
class ProxyRenderDelegate
    : public Autodesk::Maya::OPENMAYA_MPXSUBSCENEOVERRIDE_LATEST_NAMESPACE::MHWRender::
          MPxSubSceneOverride
{
    ProxyRenderDelegate(const MObject& obj);

public:
    MAYAUSD_CORE_PUBLIC
    ~ProxyRenderDelegate() override;

    MAYAUSD_CORE_PUBLIC
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/renderDelegate.h" line="33">

---

## HdVP2RenderDelegate

The `HdVP2RenderDelegate` class provides renderer-specific functionality to the render index. It is responsible for creating and deleting scene primitives, which include geometry and non-drawable objects. It provides several public methods that can be considered as endpoints, such as `GetRenderParam`, `GetSupportedRprimTypes`, `GetSupportedSprimTypes`, `GetSupportedBprimTypes`, and `GetResourceRegistry`.

```c
constexpr char VP2_RENDER_DELEGATE_SEPARATOR = ';';

PXR_NAMESPACE_OPEN_SCOPE

class HdVP2BBoxGeom;
class ProxyRenderDelegate;

/*! \brief    VP2 render delegate
    \class    HdVP2RenderDelegate

    Render delegates provide renderer-specific functionality to the render
    index, the main hydra state management structure. The render index uses
    the render delegate to create and delete scene primitives, which include
    geometry and also non-drawable objects.

    Primitives in Hydra are split into Rprims (drawables), Sprims (state
    objects like cameras and materials), and Bprims (buffer objects like
    textures). The minimum set of primitives a renderer needs to support is
    one Rprim (so the scene's not empty) and the "camera" Sprim, which is
    required by HdxRenderTask, the task implementing basic hydra drawing.

```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
