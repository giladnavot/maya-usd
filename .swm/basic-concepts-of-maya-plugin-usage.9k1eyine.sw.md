---
title: Basic Concepts of Maya Plugin Usage
---

The Maya Plugin in the maya-usd repository is used to create a bridge between Maya and Pixar's Universal Scene Description (USD). It allows for the translation of data between these two systems. The plugin is designed to be reusable and merges the best features of Pixar's USDMaya and Animal Logic's plugins. It is used to load and manage the USD data within Maya, providing a way to import, export, and manipulate USD data directly from Maya.

<SwmSnippet path="/lib/mayaUsd/fileio/registryHelper.cpp" line="124">

---

The function `_HasMayaPlugin` is used to check if a Maya plugin exists. It retrieves the metadata of the plugin and checks if it contains a Maya plugin. If it does, it retrieves the name of the Maya plugin.

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

<SwmSnippet path="/lib/mayaUsd/fileio/registryHelper.cpp" line="152">

---

The function `FindAndLoadMayaPlug` is used to find and load a Maya plugin. It iterates over all plugins and checks if they provide a type. If they do, it checks if a Maya plugin is associated with it and attempts to load it.

```c++
/* static */
void UsdMaya_RegistryHelper::FindAndLoadMayaPlug(
    const std::vector<TfToken>& scope,
    const std::string&          value)
{
    std::string         mayaPlugin;
    PlugPluginPtrVector plugins = PlugRegistry::GetInstance().GetAllPlugins();
    TF_FOR_ALL(plugIter, plugins)
    {
        PlugPluginPtr plug = *plugIter;
        if (_ProvidesForType(plug, scope, value, &mayaPlugin)) {
            if (!mayaPlugin.empty()) {
                TF_DEBUG(PXRUSDMAYA_REGISTRY)
                    .Msg(
                        "Found usdMaya plugin %s:  %s = %s.  Loading via Maya API %s.\n",
                        plug->GetName().c_str(),
                        _PluginDictScopeToDebugString(scope).c_str(),
                        value.c_str(),
                        mayaPlugin.c_str());
                std::string loadPluginCmd
                    = TfStringPrintf("loadPlugin -quiet %s", mayaPlugin.c_str());
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/plugin.cpp" line="49">

---

The function `initializePlugin` is used to initialize the Maya plugin. It registers the plugin with Maya, sets up the render tasks, and loads the necessary modules.

```c++
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
    snprintf(envVarData.data(), envVarSize, "%s", envVarSet);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/plugin.cpp" line="48">

---

# Maya Plugin Initialization

The `initializePlugin` function is the entry point for the Maya plugin. It is called when the plugin is loaded into Maya. This function performs necessary initializations, such as registering the plugin with the Maya renderer and setting up the render override.

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

<SwmSnippet path="/lib/mayaUsd/fileio/registryHelper.cpp" line="153">

---

# Maya Plugin Usage

The `FindAndLoadMayaPlug` function is an example of how the Maya plugin is used. It searches for a Maya plugin that provides a specific functionality (defined by the `scope` and `value` parameters), loads the plugin if it is not already loaded, and returns the name of the Maya plugin.

```c++
void UsdMaya_RegistryHelper::FindAndLoadMayaPlug(
    const std::vector<TfToken>& scope,
    const std::string&          value)
{
    std::string         mayaPlugin;
    PlugPluginPtrVector plugins = PlugRegistry::GetInstance().GetAllPlugins();
    TF_FOR_ALL(plugIter, plugins)
    {
        PlugPluginPtr plug = *plugIter;
        if (_ProvidesForType(plug, scope, value, &mayaPlugin)) {
            if (!mayaPlugin.empty()) {
                TF_DEBUG(PXRUSDMAYA_REGISTRY)
                    .Msg(
                        "Found usdMaya plugin %s:  %s = %s.  Loading via Maya API %s.\n",
                        plug->GetName().c_str(),
                        _PluginDictScopeToDebugString(scope).c_str(),
                        value.c_str(),
                        mayaPlugin.c_str());
                std::string loadPluginCmd
                    = TfStringPrintf("loadPlugin -quiet %s", mayaPlugin.c_str());
                if (MGlobal::executeCommand(loadPluginCmd.c_str())) {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/registryHelper.cpp" line="92">

---

# Maya Plugin Functionality

The `_ProvidesForType` function is an example of the functionality provided by the Maya plugin. It checks if a plugin provides a translator for a specific type (`typeName`). If the plugin provides the translator, the function returns the name of the Maya plugin.

```c++
static bool _ProvidesForType(
    const PlugPluginPtr&        plug,
    const std::vector<TfToken>& scope,
    const std::string&          typeName,
    std::string*                mayaPluginName)
{

    JsObject metadata = plug->GetMetadata();
    JsObject mayaTranslatorMetadata;
    if (!_ReadNestedDict(metadata, scope, &mayaTranslatorMetadata)) {
        return false;
    }

    JsValue any;
    if (!TfMapLookup(mayaTranslatorMetadata, _tokens->providesTranslator, &any)) {
        return false;
    }
    std::vector<std::string> usdTypes;
    if (!_GetData(any, &usdTypes)) {
        return false;
    }
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
