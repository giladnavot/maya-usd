---
title: Introduction to pxrUsd
---

pxrUsd is a plugin within the maya-usd repository. It is responsible for integrating Pixar's Universal Scene Description (USD) with Maya. It provides the functionality to import and export USD data from and to Maya. The plugin is defined in the `plugin.cpp` file, where it registers various commands, nodes, and file translators to Maya's plugin architecture. The `pxrUsd` plugin also includes a set of API macros defined in `api.h` for managing the plugin's export and import functionality.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="46">

---

# Initialization of pxrUsd

This is the `initializePlugin` function, which is called when the plugin is loaded into Maya. It initializes the various components of the plugin, such as the UFE, proxy shape, and reference assembly. It also registers the necessary commands and translators with Maya.

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

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/api.h" line="21">

---

# Definition of PXRUSD_API

These are the definitions of the `PXRUSD_API` macro, which is used to control the visibility of symbols in the library. Depending on whether the library is being built statically or dynamically, it will either export or import the symbols.

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

# Definition of PXRUSDMAYA_DEFINE_WRITER

This is the definition of the `PXRUSDMAYA_DEFINE_WRITER` macro, which is used to define a writer for a specific Maya node type. The writer is responsible for converting the Maya node into a USD prim when exporting to USD.

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

PXR_NAMESPACE_CLOSE_SCOPE
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
