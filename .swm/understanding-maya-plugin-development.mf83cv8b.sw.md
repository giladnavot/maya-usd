---
title: Understanding Maya Plugin Development
---

Maya Plugin Development involves creating plugins that extend the functionality of Autodesk's Maya software. In the context of the maya-usd repository, this involves creating plugins that allow for the use of Pixar's Universal Scene Description (USD) within Maya. This is achieved through a combination of C++ and Python code that interfaces with both the Maya and USD APIs. The plugins provide functionality such as importing and exporting USD files, translating between Maya and USD data structures, and providing a USD stage that can be viewed and manipulated within Maya.

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/plugin.cpp" line="48">

---

# Plugin Initialization

The `initializePlugin` function is a crucial part of Maya plugin development. This function is called when the plugin is loaded into Maya. It is responsible for setting up any necessary data structures, registering commands, nodes, or any other elements that the plugin provides. In the maya-usd codebase, the `initializePlugin` function is used to register the plugin with Maya, initialize the HdMayaAdapter, and set up the render overrides.

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

The `uninitializePlugin` function is called when the plugin is unloaded from Maya. This function is responsible for cleaning up anything that was set up in `initializePlugin`. In the maya-usd codebase, this function deregisters the commands and render overrides that were set up during initialization.

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

<SwmSnippet path="/lib/mayaUsd/utils/plugInfo.json" line="1">

---

# Plugin Metadata

The `plugInfo.json` file provides metadata about the plugin. This includes information about the plugin's name, type, and any additional metadata that might be required. In the maya-usd codebase, this file is used to specify metadata related to selectability and lock properties.

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

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/pluginDebugCodes.h" line="1">

---

# Plugin Debugging

Debugging is an important part of plugin development. The `pluginDebugCodes.h` file defines several debug codes that can be used to control the output of debug information. This can be useful for tracking down issues and understanding the behavior of the plugin.

```c
//
// Copyright 2019 Luma Pictures
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef HDMAYA_PLUGIN_DEBUG_CODES_H
#define HDMAYA_PLUGIN_DEBUG_CODES_H

#include <pxr/base/tf/debug.h>
#include <pxr/pxr.h>

```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
