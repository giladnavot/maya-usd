---
title: Overview of pxrUsd Plugin
---

The pxrUsd plugin is a part of the Maya-USD project that registers Pixar's USD functionality in Maya. This includes import and export commands, as well as USD nodes such as Pixar's USD reference assembly and the Pixar-specific USD proxy shape. The plugin is implemented in the UsdMaya library and should be favored over the mayaUsdPlugin. The plugin also contains a method called initializePlugin which is used to initialize the plugin and register various shapes, nodes, and commands. It also includes a method called append which is used to keep the Maya path in sync with the USD prim.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="46">

---

# Initialization of pxrUsd Plugin

The `initializePlugin` function in `plugin.cpp` is responsible for initializing the pxrUsd plugin. This includes initializing the UFE (Universal Front End), registering the proxy shape and reference assembly nodes, sourcing the `usdMaya.mel` file, and registering various commands and file translators. This function is called when the plugin is loaded into Maya.

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

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugInfo.json" line="1">

---

# pxrUsd Plugin Registration

The `plugInfo.json` file contains information about the pxrUsd plugin, including the name of the plugin and the functionality it provides. This information is used by Maya to register the plugin.

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

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/testenv/testPxrUsdAlembicChaser.py" line="31">

---

# pxrUsd Plugin Usage

The `testPxrUsdAlembicChaser.py` file provides an example of how the pxrUsd plugin can be used. In this case, the plugin is loaded into Maya and used to export a USD file with specific attributes and primvars.

```python
class testUserExportedAttributes(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        standalone.initialize('usd')

        # The alembic chaser is in the pxrUsd plugin.
        cmds.loadPlugin('pxrUsd', quiet=True)

    @classmethod
    def tearDownClass(cls):
        standalone.uninitialize()

    def testExportAttributes(self):
        mayaFilePath = os.path.abspath('AlembicChaser.ma')
        cmds.file(mayaFilePath, open=True, force=True)

        usdFilePath = os.path.abspath('out.usda')

        # Export to USD.
        cmds.usdExport(
```

---

</SwmSnippet>

# Functions of pxrUsd Plugin

The pxrUsd plugin provides a set of functionalities for Pixar's Universal Scene Description (USD) in Maya.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/usdTranslator.cpp" line="22">

---

## PXRUSDMAYA_DEFINE_WRITER

The `PXRUSDMAYA_DEFINE_WRITER` function is a macro that defines a writer for a specific type of USD object. It is used to create a writer for the `pxrUsdReferenceAssembly` and `pxrUsdProxyShape` types.

```c++
PXR_NAMESPACE_OPEN_SCOPE

PXRUSDMAYA_DEFINE_WRITER(pxrUsdReferenceAssembly, args, context)
{
    return UsdMayaTranslatorModelAssembly::Create(args, context);
}

PXRUSDMAYA_DEFINE_WRITER(pxrUsdProxyShape, args, context)
{
    return UsdMayaTranslatorModelAssembly::Create(args, context);
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="46">

---

## initializePlugin

The `initializePlugin` function is used to initialize the pxrUsd plugin. It registers the USD functionality in Maya, including import and export commands, as well as USD nodes such as Pixar's USD reference assembly and the Pixar-specific USD proxy shape.

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

# pxrUsd Plugin Endpoints

Understanding pxrUsd Plugin Endpoints

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="47">

---

## initializePlugin

The `initializePlugin` function is the entry point for the pxrUsd plugin. It is responsible for initializing the plugin, registering shapes, nodes, commands, and file translators. It also sets up listeners and loads additional plugins.

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

The `uninitializePlugin` function is called when the plugin is being unloaded. It is responsible for deregistering commands, nodes, and file translators that were registered during initialization. It also removes listeners and performs other cleanup tasks.

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
