---
title: Basic Concepts of pxrUsd
---

pxrUsd is a plugin in the maya-usd repository that provides a bridge between Pixar's Universal Scene Description (USD) and Maya. It includes functionalities for importing and exporting USD files in Maya, and also provides a set of APIs for manipulating and interacting with USD data. The plugin is defined in the `plugin.cpp` file, which includes the `initializePlugin` function to set up the plugin in Maya. It also includes a set of macros defined in `api.h` for handling API exports and imports. The `usdTranslator.cpp` file defines the `PXRUSDMAYA_DEFINE_WRITER` function, which is used for creating USD writers for different types of Maya nodes.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="46">

---

# pxrUsd Initialization

The `initializePlugin` function is a key part of pxrUsd. It is responsible for initializing the plugin when it is loaded into Maya. This includes registering the various nodes, shapes, and commands provided by the plugin, as well as initializing the USD and UFE libraries.

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

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/usdTranslator.cpp" line="24">

---

# pxrUsd Translation

The `PXRUSDMAYA_DEFINE_WRITER` macro is used to define writers for different types of nodes. These writers are responsible for translating the data from Maya nodes into USD prims.

```c++
PXRUSDMAYA_DEFINE_WRITER(pxrUsdReferenceAssembly, args, context)
{
    return UsdMayaTranslatorModelAssembly::Create(args, context);
}

PXRUSDMAYA_DEFINE_WRITER(pxrUsdProxyShape, args, context)
{
    return UsdMayaTranslatorModelAssembly::Create(args, context);
}

PXR_NAMESPACE_CLOSE_SCOPE
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/api.h" line="21">

---

# pxrUsd API

The `PXRUSD_API` macro is used to control the visibility of the functions and classes defined in the pxrUsd library. Depending on whether the library is being compiled as a static or dynamic library, and whether it is being compiled for export or import, different visibility attributes are applied.

```c
#if defined(PXR_STATIC)
#define PXRUSD_API
#define PXRUSD_API_TEMPLATE_CLASS(...)
#define PXRUSD_API_TEMPLATE_STRUCT(...)
#define PXRUSD_LOCAL
#else
#if defined(PXRUSD_EXPORTS)
#define PXRUSD_API                      ARCH_EXPORT
#define PXRUSD_API_TEMPLATE_CLASS(...)  ARCH_EXPORT_TEMPLATE(class, __VA_ARGS__)
#define PXRUSD_API_TEMPLATE_STRUCT(...) ARCH_EXPORT_TEMPLATE(struct, __VA_ARGS__)
#else
#define PXRUSD_API                      ARCH_IMPORT
#define PXRUSD_API_TEMPLATE_CLASS(...)  ARCH_IMPORT_TEMPLATE(class, __VA_ARGS__)
#define PXRUSD_API_TEMPLATE_STRUCT(...) ARCH_IMPORT_TEMPLATE(struct, __VA_ARGS__)
#endif
#define PXRUSD_LOCAL ARCH_HIDDEN
#endif
```

---

</SwmSnippet>

# pxrUsd Functions

This section will cover the main functions of pxrUsd, including PXRUSD_API, PXRUSDMAYA_DEFINE_WRITER, and initializePlugin.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/api.h" line="21">

---

## PXRUSD_API

The `PXRUSD_API` macro is used for exporting symbols in the pxrUsd plugin. It is defined differently depending on whether the plugin is being built statically or dynamically.

```c
#if defined(PXR_STATIC)
#define PXRUSD_API
#define PXRUSD_API_TEMPLATE_CLASS(...)
#define PXRUSD_API_TEMPLATE_STRUCT(...)
#define PXRUSD_LOCAL
#else
#if defined(PXRUSD_EXPORTS)
#define PXRUSD_API                      ARCH_EXPORT
#define PXRUSD_API_TEMPLATE_CLASS(...)  ARCH_EXPORT_TEMPLATE(class, __VA_ARGS__)
#define PXRUSD_API_TEMPLATE_STRUCT(...) ARCH_EXPORT_TEMPLATE(struct, __VA_ARGS__)
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/usdTranslator.cpp" line="22">

---

## PXRUSDMAYA_DEFINE_WRITER

The `PXRUSDMAYA_DEFINE_WRITER` function is used to define a writer for a specific type of USD reference. It is used to create a translator for the pxrUsdReferenceAssembly and pxrUsdProxyShape.

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

The `initializePlugin` function is used to initialize the pxrUsd plugin in Maya. It registers the plugin, initializes the UFE (Universal Front End), and sets up the proxy shape and reference assembly nodes.

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

Understanding pxrUsd Plugin Initialization and Uninitialization

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="47">

---

## initializePlugin

The `initializePlugin` function is the entry point when the plugin is loaded into Maya. It performs several tasks such as initializing UFE, registering the plugin's nodes and commands, sourcing MEL scripts, and setting up listeners for various events.

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

The `uninitializePlugin` function is called when the plugin is unloaded from Maya. It performs cleanup tasks such as deregistering the plugin's nodes and commands, executing cleanup commands, and removing event listeners.

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
