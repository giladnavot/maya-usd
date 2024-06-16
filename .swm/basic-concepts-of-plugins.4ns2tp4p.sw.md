---
title: Basic Concepts of Plugins
---

Plugins in the maya-usd repository are modules that add specific features to the Maya software, enhancing its functionality. They are primarily used to integrate Pixar's Universal Scene Description (USD) functionality into Maya. This includes import and export commands, as well as USD nodes such as Pixar's USD reference assembly and the Pixar-specific USD proxy shape. The plugins are initialized and finalized using the `initializePlugin` and `uninitializePlugin` functions respectively. They also register various commands, nodes, and file translators to Maya during initialization.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="46">

---

# Plugin Registration

This is the `initializePlugin` function, which is called when the plugin is loaded into Maya. It registers various features of the plugin, such as shapes, nodes, commands, and file translators, with Maya's plugin manager.

```c++
PXRUSD_API
MStatus initializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj, "Pixar", "1.0", "Any");

    status = MayaUsd::ufe::initialize();
    if (!status) {
        status.perror("Unable to initialize ufe.");
    }

    status = MayaUsdProxyShapePlugin::initialize(plugin);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = plugin.registerShape(
        UsdMayaProxyShape::typeName,
        UsdMayaProxyShape::typeId,
        UsdMayaProxyShape::creator,
        UsdMayaProxyShape::initialize,
        UsdMayaProxyShapeUI::creator,
        MayaUsdProxyShapePlugin::getProxyShapeClassification());
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="26">

---

# Plugin Namespace

This is the namespace declaration for the plugin. It contains constants that define the plugin's type name and type ID, which are used to identify the plugin in the software.

```c++
PXR_NAMESPACE_USING_DIRECTIVE

namespace {
const MString  PxrUsdPreviewSurface_typeName("pxrUsdPreviewSurface");
const MTypeId  PxrUsdPreviewSurface_typeId(0x00126403);
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugInfo.json" line="1">

---

# Plugin Information

This is the `plugInfo.json` file, which provides metadata about the plugin. It includes information such as the plugin's name, type, and the features it provides.

```json
{
  "Plugins": [
    {
      "Info": {
        "UsdMaya": {
          "PrimWriter": {
            "mayaPlugin": "pxrUsd",
            "providesTranslator": [
              "pxrUsdProxyShape",
              "pxrUsdReferenceAssembly"
            ]
          }
        }
      },
      "Name": "@PLUG_INFO_PLUGIN_NAME@",
      "Type": "resource"
    }
  ]
}
```

---

</SwmSnippet>

# Plugin Overview

This is the overview document for the plugin. It provides a high-level description of the plugin's functionality and its role in the software.

# Plugin Endpoints

Understanding Plugin Endpoints

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="47">

---

## initializePlugin

The `initializePlugin` function is a crucial endpoint in a Maya plugin. It is called when the plugin is loaded into Maya. It is responsible for registering the plugin's functionalities with Maya, such as shapes, nodes, commands, and file translators. For example, in the pxrUsd plugin, it initializes the UFE (Universal Front End), registers the ProxyShape and ReferenceAssembly nodes, sources the usdMaya.mel file, and registers several commands and file translators.

```c++
MStatus initializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj, "Pixar", "1.0", "Any");

    status = MayaUsd::ufe::initialize();
    if (!status) {
        status.perror("Unable to initialize ufe.");
    }

    status = MayaUsdProxyShapePlugin::initialize(plugin);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = plugin.registerShape(
        UsdMayaProxyShape::typeName,
        UsdMayaProxyShape::typeId,
        UsdMayaProxyShape::creator,
        UsdMayaProxyShape::initialize,
        UsdMayaProxyShapeUI::creator,
        MayaUsdProxyShapePlugin::getProxyShapeClassification());
    CHECK_MSTATUS_AND_RETURN_IT(status);
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="188">

---

## uninitializePlugin

The `uninitializePlugin` function is another important endpoint in a Maya plugin. It is called when the plugin is unloaded from Maya. It is responsible for deregistering the plugin's functionalities from Maya, effectively cleaning up the changes made during the plugin's initialization. In the pxrUsd plugin, it finalizes the UFE, deregisters the ProxyShape and ReferenceAssembly nodes, and deregisters several commands and file translators.

```c++
MStatus uninitializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj);

    status = MayaUsd::ufe::finalize();
    CHECK_MSTATUS(status);

    status = plugin.deregisterCommand("usdImport");
    if (!status) {
        status.perror("deregisterCommand usdImport");
    }

    status = plugin.deregisterCommand("usdExport");
    if (!status) {
        status.perror("deregisterCommand usdExport");
    }

    status = plugin.deregisterCommand("usdListShadingModes");
    if (!status) {
        status.perror("deregisterCommand usdListShadingModes");
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
