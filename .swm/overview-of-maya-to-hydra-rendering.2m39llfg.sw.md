---
title: Overview of Maya to Hydra Rendering
---
Maya to Hydra Rendering in the maya-usd repository refers to the process of translating Maya's scene data into a format that Hydra, the rendering subsystem in USD, can understand and render. This process is facilitated by a set of classes and functions in the `mayaToHydra` directory. The `MtohRenderOverride` class, for instance, is a key component that manages the rendering process, including setting up render operations, managing render resources, and handling selection changes. The `MtohRenderGlobals` class is responsible for managing global render settings. The `setup` function in `MtohRenderOverride` is a crucial function that sets up the render operations and callbacks for a given render panel. The `MtohViewCmd` class provides a command interface for managing Hydra views in Maya. The `MtohRendererDescription` struct provides a description of a Hydra renderer plugin, including its name and display name.

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderGlobals.h" line="36">

---

# MtohRenderGlobals

The `MtohRenderGlobals` class is responsible for managing the render globals for the Maya to Hydra process. It includes methods for creating attributes, applying settings, and building options menus for the renderer.

```c
class MtohRenderGlobals
{
public:
    MtohRenderGlobals();
    ~MtohRenderGlobals() = default;

    struct GlobalParams
    {
        const TfToken filter = {};
        // Is the filter above only a renderer, or a renderer.attribute
        const bool filterIsRenderer = false;
        // If creating the attribute for the first time, immediately set to a user default
        const bool fallbackToUserDefaults = true;
        // TODO: Extend this and mtoh with a setting to ignore scene settings

        GlobalParams() = default;
        GlobalParams(const TfToken f, bool fir, bool ftud)
            : filter(f)
            , filterIsRenderer(fir)
            , fallbackToUserDefaults(ftud)
        {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverride.h" line="50">

---

# MtohRenderOverride

The `MtohRenderOverride` class is a key component of the Maya to Hydra rendering process. It handles the rendering operations, manages the render delegates, and updates the render globals. It also handles the selection and highlighting of objects in the viewport.

```c
class MtohRenderOverride : public MHWRender::MRenderOverride
{
public:
    MtohRenderOverride(const MtohRendererDescription& desc);
    ~MtohRenderOverride() override;

    /// Mark a setting (or all settings when attrName is '') as out of date
    static void UpdateRenderGlobals(const MtohRenderGlobals& globals, const TfToken& attrName = {});

    /// The names of all render delegates that are being used by at least
    /// one modelEditor panel.
    static std::vector<MString> AllActiveRendererNames();

    /// Returns a list of rprims in the render index for the given render
    /// delegate.
    ///
    /// Intended mostly for use in debugging and testing.
    static SdfPathVector RendererRprims(TfToken rendererName, bool visibleOnly = false);

    /// Returns the scene delegate id for the given render delegate and
    /// scene delegate names.
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/viewCommand.h" line="25">

---

# MtohViewCmd

The `MtohViewCmd` class is a command class that provides various utility functions related to the Maya to Hydra rendering process. It includes methods for listing renderers, delegates, and updating render globals.

```c
class MtohViewCmd : public MPxCommand
{
public:
    static void*   creator() { return new MtohViewCmd(); }
    static MSyntax createSyntax();

    static const MString name;

    MStatus doIt(const MArgList& args) override;
};
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/utils.cpp" line="33">

---

# MtohInitializeRenderPlugins

The `MtohInitializeRenderPlugins` function is used to initialize the render plugins for the Maya to Hydra rendering process. It retrieves the plugin descriptions and creates the render delegates.

```c++
std::pair<const MtohRendererDescriptionVector&, const MtohRendererSettings&>
MtohInitializeRenderPlugins()
{
    using Storage = std::pair<MtohRendererDescriptionVector, MtohRendererSettings>;

    static const Storage ret = []() -> Storage {
        HdRendererPluginRegistry& pluginRegistry = HdRendererPluginRegistry::GetInstance();
        HfPluginDescVector        pluginDescs;
        pluginRegistry.GetPluginDescs(&pluginDescs);

        Storage store;
        store.first.reserve(pluginDescs.size());

        MtohRenderGlobals::OptionsPreamble();

        for (const auto& pluginDesc : pluginDescs) {
            const TfToken     renderer = pluginDesc.id;
            HdRendererPlugin* plugin = pluginRegistry.GetRendererPlugin(renderer);
            if (!plugin) {
                continue;
            }
```

---

</SwmSnippet>

# Maya to Hydra Rendering Functions

This section covers the main functions involved in the Maya to Hydra rendering process.

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverrideUtils.h" line="25">

---

## HdMayaPreRender

The `HdMayaPreRender` class is a pre-render operation in the Maya to Hydra rendering process. It sets up the clear operation with the renderer's gradient and clear colors. It is used in the `renderOverride.cpp` file to clear and draw the grid.

```c
class HdMayaPreRender : public MHWRender::MSceneRender
{
public:
    explicit HdMayaPreRender(const MString& name)
        : MHWRender::MSceneRender(name)
    {
        auto*      renderer = MHWRender::MRenderer::theRenderer();
        const auto gradient = renderer->useGradient();
        const auto color1 = renderer->clearColor();
        const auto color2 = renderer->clearColor2();

        float c1[4] = { color1[0], color1[1], color1[2], 1.0f };
        float c2[4] = { color2[0], color2[1], color2[2], 1.0f };

        mClearOperation.setClearColor(c1);
        mClearOperation.setClearColor2(c2);
        mClearOperation.setClearGradient(gradient);
    }

    MSceneFilterOption renderFilterOverride() override { return kRenderPreSceneUIItems; }

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverrideUtils.h" line="49">

---

## HdMayaPostRender

The `HdMayaPostRender` class is a post-render operation in the Maya to Hydra rendering process. It sets the clear operation mask to none and defines the object type exclusions. It is used in the `renderOverride.cpp` file to draw scene elements.

```c
class HdMayaPostRender : public MHWRender::MSceneRender
{
public:
    explicit HdMayaPostRender(const MString& name)
        : MHWRender::MSceneRender(name)
    {
        mClearOperation.setMask(MHWRender::MClearOperation::kClearNone);
    }

    MUint64 getObjectTypeExclusions() override
    {
        // FIXME:
        //   1. kExcludePluginShapes is here so as to not re-draw UsdProxy shapes
        //      ...but that means no plugin shapes would be drawn.
        //   2. Curves as controls and curves as a renderitem need to be delineated
        //
        return MFrameContext::kExcludeMeshes | MFrameContext::kExcludePluginShapes;
    }

    MSceneFilterOption renderFilterOverride() override
    {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverrideUtils.h" line="76">

---

## HdMayaRender

The `HdMayaRender` class is the main render operation in the Maya to Hydra rendering process. It executes the render operation with the provided draw context. It is used in the `renderOverride.cpp` file for the main Hydra render.

```c
class HdMayaRender : public MHWRender::MUserRenderOperation
{
public:
    HdMayaRender(const MString& name, MtohRenderOverride* override)
        : MHWRender::MUserRenderOperation(name)
        , _override(override)
    {
    }

    MStatus execute(const MHWRender::MDrawContext& drawContext) override
    {
        return _override->Render(drawContext);
    }

private:
    MtohRenderOverride* _override;
};
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverrideUtils.h" line="161">

---

## HdMayaSetRenderGLState

The `HdMayaSetRenderGLState` class sets the render GL state for the Maya to Hydra rendering process. It sets the blend function, blend equation, and enables or disables the blend and cull face based on the old state. It is used in the `renderOverride.cpp` file to set the render GL state.

```c
class HdMayaSetRenderGLState
{
public:
    HdMayaSetRenderGLState()
    {
        glGetIntegerv(GL_BLEND_SRC_ALPHA, &_oldBlendFunc);
        glGetIntegerv(GL_BLEND_EQUATION_RGB, &_oldBlendEquation);
        glGetBooleanv(GL_BLEND, &_oldBlend);
        glGetBooleanv(GL_CULL_FACE, &_oldCullFace);

        if (_oldBlendFunc != BLEND_FUNC) {
            glBlendFunc(GL_SRC_ALPHA, BLEND_FUNC);
        }

        if (_oldBlendEquation != BLEND_EQUATION) {
            glBlendEquation(BLEND_EQUATION);
        }

        if (_oldBlend != BLEND) {
            glEnable(GL_BLEND);
        }
```

---

</SwmSnippet>

# Maya to Hydra Rendering

Understanding Maya to Hydra Rendering

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverride.cpp" line="436">

---

## MtohRenderOverride::Render

The `MtohRenderOverride::Render` function is a key endpoint in the Maya to Hydra rendering process. It is responsible for executing the rendering process within the context of Maya's draw context. This function handles various aspects of the rendering process, including managing the render delegate, updating render globals, and handling selection changes.

```c++
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
        // and continually execute it until the renderer signals converged, at which point
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverride.cpp" line="282">

---

## MtohRenderOverride::\_GetRenderDelegate

The `MtohRenderOverride::_GetRenderDelegate` function is another important endpoint in the Maya to Hydra rendering process. It is responsible for retrieving the render delegate, which is an object that provides services for the rendering process, such as resource allocation and synchronization.

```c++
HdRenderDelegate* MtohRenderOverride::_GetRenderDelegate()
{
    return _renderIndex ? _renderIndex->GetRenderDelegate() : nullptr;
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
