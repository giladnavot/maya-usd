---
title: Getting Started with Maya to Hydra Rendering
---
Maya to Hydra Rendering in the maya-usd repository refers to the process of translating Maya's scene data into a format that Hydra, the rendering architecture of USD (Universal Scene Description), can understand and render. This process involves creating a Maya plugin and reusable libraries that merge the features of Pixar's USDMaya and Animal Logic's plugins. The 'mayaToHydra' directory contains the code for this process, including the render settings, render override, and utilities for the rendering process. The rendering process is managed by the MtohRenderOverride class, which controls the rendering operations. The MtohRenderGlobals class manages the render settings, and the utilities provide additional functionalities such as renderer descriptions and settings.

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderOverride.h" line="50">

---

# MtohRenderOverride Class

The MtohRenderOverride class is the main class responsible for the Maya to Hydra Rendering process. It overrides Maya's default viewport rendering, instead using Hydra to render the viewport. This class contains methods for setting up the render, cleaning up after the render, and performing the render itself.

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

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderGlobals.h" line="36">

---

# MtohRenderGlobals Class

The MtohRenderGlobals class is used to manage global render settings for the Maya to Hydra Rendering process. It provides methods for creating and updating render globals, which are settings that affect the entire render.

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

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/viewCommand.h" line="25">

---

# MtohViewCmd Class

The MtohViewCmd class is a command class that provides a Maya command for interacting with the Maya to Hydra Rendering process. This command can be used to list available renderers, get the display name of a renderer, and more.

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

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
