---
title: Introduction to Maya Plugin Development
---

Maya Plugin Development involves creating plugins that extend the functionality of Autodesk's Maya software. In the context of the maya-usd repository, this involves creating plugins that allow for the use of Pixar's Universal Scene Description (USD) within Maya. This includes creating and managing USD stages, reading and writing USD files, and translating between Maya and USD data. The codebase includes various utilities and classes for handling USD data, as well as interfaces for interacting with Maya's plugin system. For example, the `initializePlugin` function in `lib/mayaUsd/render/mayaToHydra/plugin.cpp` is a key part of setting up a Maya plugin, registering the plugin with Maya's plugin system and setting up necessary render tasks.

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/plugin.cpp" line="48">

---

# Plugin Initialization

The `initializePlugin` function is a crucial part of Maya plugin development. This function is called when the plugin is loaded into Maya, and it is where the plugin sets up any necessary resources it needs. In this case, the function is registering the plugin with Maya, initializing the HdMayaAdapter, and setting up the render overrides.

```c++
PLUGIN_EXPORT MStatus initializePlugin(MObject obj)
{
    MString experimental("Maya to Hydra (mtoh) is experimental.");
    MGlobal::displayWarning(experimental);

    MStatus ret = MS::kSuccess;

    // Call one time registration of plugins compiled for same USD version as MayaUSD plugin.
    MayaUsd::registerVersionedPlugins();

    ret = HdMayaAdapter::Initialize();
    if (!ret) {
        return ret;
    }

    // For now this is required for the HdSt backed to use lights.
    // putenv requires char* and I'm not willing to use const cast!
    constexpr const char* envVarSet = "USDIMAGING_ENABLE_SCENE_LIGHTS=1";
    const auto            envVarSize = strlen(envVarSet) + 1;
    std::vector<char>     envVarData;
    envVarData.resize(envVarSize);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/plugin.cpp" line="95">

---

# Plugin Uninitialization

The `uninitializePlugin` function is called when the plugin is unloaded from Maya. This is where the plugin should clean up any resources it created during initialization to prevent memory leaks. Here, the function is deregistering the plugin from Maya and clearing the render overrides.

```c++
PLUGIN_EXPORT MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin plugin(obj, "Autodesk", TOSTRING(MAYAUSD_VERSION), "Any");
    MStatus   ret = MS::kSuccess;
    if (auto* renderer = MHWRender::MRenderer::theRenderer()) {
        for (unsigned int i = 0; i < gsRenderOverrides.size(); i++) {
            renderer->deregisterOverride(gsRenderOverrides[i].get());
            gsRenderOverrides[i] = nullptr;
        }
    }

    // Note: when Maya is doing its default "quick exit" that does not uninitialize plugins,
    //       these overrides crash on destruction because Hydra ha already destroyed things
    //       these rely on. There is not much we can do about it...
    gsRenderOverrides.clear();

    // Clear any registered callbacks
    MGlobal::executeCommand("callbacks -cc mtoh;");

    if (!plugin.deregisterCommand(MtohViewCmd::name)) {
        ret = MS::kFailure;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/registryHelper.cpp" line="124">

---

# Plugin Functionality

The `_HasMayaPlugin` function is an example of the functionality a plugin might provide. This function checks if a specific Maya plugin is available. Functions like these are what give the plugin its functionality and allow it to interact with Maya.

```c++
static bool _HasMayaPlugin(
    const PlugPluginPtr&        plug,
    const std::vector<TfToken>& scope,
    std::string*                mayaPluginName)
{
    JsObject metadata = plug->GetMetadata();
    JsObject mayaTranslatorMetadata;
    if (!_ReadNestedDict(metadata, scope, &mayaTranslatorMetadata)) {
        return false;
    }

    JsValue any;
    if (TfMapLookup(mayaTranslatorMetadata, _tokens->mayaPlugin, &any)) {
        // Find the mayaPlugin if there is one. Otherwise we can still load the plugin via USD.
        _GetData(any, mayaPluginName);
    }

    return true;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/utils/plugInfo.json" line="1">

---

# Plugin Metadata

The `plugInfo.json` file is used to store metadata about the plugin. This includes information such as the plugin's name, type, and any additional information required by the plugin. This metadata is used by Maya to understand how to interact with the plugin.

```json
# plugInfo.json
{
    "Plugins": [
        {
            "Info": {
                "SdfMetadata": {
                    # Selectability controls Maya viewport selectability with the mouse.
                    # If set, can be "on" (selectable) or "off" (unselectable).
                    # If missing, defaults to "on" (selectable).
                    # It is enforced via the ProxyRenderDelegate via its getInstancedSelectionPath()
                    # function.
                    "mayaSelectability": {
                        "type": "token",
                        "appliesTo": "prims"
                    },
                    # Lock on properties.
                    # If set, can be "off" (editable) or "on" (locked).
                    # If missing, defaults to "off" (editable).
                    "mayaLock": {
                        "type": "token",
                        "appliesTo": "properties",
```

---

</SwmSnippet>

# Functions in Maya Plugin Development

This section covers the main functions involved in Maya Plugin Development in the maya-usd repository.

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeUseRegistry.cpp" line="156">

---

## \_ExportShadingDepGraph Function

The `_ExportShadingDepGraph` function is used to export nodes in the Maya dependency graph under a specified material export path. The function takes in the material export path, a root plug from an attribute on the Maya shadingEngine node that the material represents, and the context. It maintains a mapping of Maya shading node handles to shader writers to ensure each shader is only authored once.

```c++
    /// Export nodes in the Maya dependency graph rooted at \p rootPlug
    /// under \p materialExportPath.
    ///
    /// The root plug should be from an attribute on the Maya shadingEngine
    /// node that the material represents.
    ///
    /// The first shader prim authored during the traversal will be assumed
    /// to be the primary shader for the connection represented by
    /// \p rootPlug. That shader prim will be returned so that it can be
    /// connected to the Material prim.
    UsdShadeShader _ExportShadingDepGraph(
        const SdfPath&                         materialExportPath,
        const MPlug&                           rootPlug,
        const UsdMayaShadingModeExportContext& context)
    {
        // Maintain a mapping of Maya shading node handles to shader
        // writers so that we only author each shader once, but can still
        // look them up again to create connections.
        _NodeHandleToShaderWriterMap shaderWriterMap;

        // MItDependencyGraph takes a non-const MPlug as a constructor
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorLight.cpp" line="138">

---

## \_ReadLightAttrs Function

The `_ReadLightAttrs` function is used to import the common light attributes from UsdLuxLightAPI. It reads a Usd attribute, accounting for eventual animations. It also sets Maya's light 'useRayTracedShadows' if the USD shadows are enabled.

```c++
// Import the common light attributes from UsdLuxLightAPI.
// As opposed to the writer, we can't rely on the MFnLight attribute
// accessors, as we need to support animations. Instead we're getting
// the Maya plugs from MFnDependencyNode
static bool _ReadLightAttrs(
    const UsdLuxLightAPI&        lightSchema,
    MFnDependencyNode&           depFn,
    const UsdMayaPrimReaderArgs& args,
    UsdMayaPrimReaderContext&    context)
{
    MStatus status;
    bool    success = true;
    UsdPrim prim = lightSchema.GetPrim();

    // We need to specify a time when getting an attribute, otherwise we can get invalid data
    // for single time-samples
    UsdTimeCode timeCode(args.GetTimeInterval().GetMin());

    // ReadUsdAttribute will read a Usd attribute, accounting for eventual animations
    success &= UsdMayaReadUtil::ReadUsdAttribute(
        lightSchema.GetIntensityAttr(), depFn, _tokens->IntensityPlugName, args, &context);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/plugin.cpp" line="48">

---

## initializePlugin Function

The `initializePlugin` function is used to initialize the Maya to Hydra plugin. It registers the plugin with the Maya renderer, sets up the render tasks, and loads the plugin.

```c++
PLUGIN_EXPORT MStatus initializePlugin(MObject obj)
{
    MString experimental("Maya to Hydra (mtoh) is experimental.");
    MGlobal::displayWarning(experimental);

    MStatus ret = MS::kSuccess;

    // Call one time registration of plugins compiled for same USD version as MayaUSD plugin.
    MayaUsd::registerVersionedPlugins();

    ret = HdMayaAdapter::Initialize();
    if (!ret) {
        return ret;
    }

    // For now this is required for the HdSt backed to use lights.
    // putenv requires char* and I'm not willing to use const cast!
    constexpr const char* envVarSet = "USDIMAGING_ENABLE_SCENE_LIGHTS=1";
    const auto            envVarSize = strlen(envVarSet) + 1;
    std::vector<char>     envVarData;
    envVarData.resize(envVarSize);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/utils/plugRegistryHelper.cpp" line="231">

---

## registerVersionedPlugins Function

The `registerVersionedPlugins` function is used to register USD plugins with USD/MayaUsd/Python version checks. It ensures that each plugin is only registered once.

```c++
void registerVersionedPlugins()
{
    static std::once_flag once;
    std::call_once(once, []() {
        const std::string pythonVersion = TOSTRING(MAYA_PY_VERSION);
        const std::string usdVersion = TOSTRING(ADSK_USD_VERSION);
        const std::string mayaUsdVersion = TOSTRING(MAYAUSD_VERSION);

        std::vector<std::string> pluginsToRegister;

        const std::string paths = TfGetenv("MAYA_PXR_PLUGINPATH_NAME");
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
