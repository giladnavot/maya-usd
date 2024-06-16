---
title: Introduction to pxrUsdPreviewSurface
---

The `pxrUsdPreviewSurface` is a part of the Maya plugin in the 'maya-usd' repository. It is used to create a preview surface for Pixar's Universal Scene Description (USD) in Maya. The `pxrUsdPreviewSurface` is initialized through the `initializePlugin` method, which uses the `PxrMayaUsdPreviewSurfacePlugin::initialize` function. This function takes in the plugin, the type name, type ID, and registrant ID of the `pxrUsdPreviewSurface`. The type name and registrant ID are defined as `pxrUsdPreviewSurface`, and the type ID is a unique identifier for the `pxrUsdPreviewSurface`.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="43">

---

# Initializing the pxrUsdPreviewSurface Plugin

Here we see the `initializePlugin` function, which is used to set up the pxrUsdPreviewSurface plugin in Maya. It takes in a `MObject`, sets up a `MFnPlugin` with the necessary information, and then calls `PxrMayaUsdPreviewSurfacePlugin::initialize` to complete the setup.

```c++
PXRUSDPREVIEWSURFACE_API
MStatus initializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj, "Pixar", "1.0", "Any");

    status = PxrMayaUsdPreviewSurfacePlugin::initialize(
        plugin,
        PxrUsdPreviewSurface_typeName,
        PxrUsdPreviewSurface_typeId,
        PxrUsdPreviewSurface_registrantId);
    CHECK_MSTATUS(status);

    return MS::kSuccess;
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/api.h" line="21">

---

# pxrUsdPreviewSurface API

This file defines the API for pxrUsdPreviewSurface. It includes macros for exporting and importing the plugin, as well as for defining template classes and structures. These are used to ensure the plugin is correctly set up and can interact with other parts of the codebase.

```c
#if defined(PXR_STATIC)
#define PXRUSDPREVIEWSURFACE_API
#define PXRUSDPREVIEWSURFACE_API_TEMPLATE_CLASS(...)
#define PXRUSDPREVIEWSURFACE_API_TEMPLATE_STRUCT(...)
#define PXRUSDPREVIEWSURFACE_LOCAL
#else
#if defined(PXRUSDPREVIEWSURFACE_EXPORTS)
#define PXRUSDPREVIEWSURFACE_API                      ARCH_EXPORT
#define PXRUSDPREVIEWSURFACE_API_TEMPLATE_CLASS(...)  ARCH_EXPORT_TEMPLATE(class, __VA_ARGS__)
#define PXRUSDPREVIEWSURFACE_API_TEMPLATE_STRUCT(...) ARCH_EXPORT_TEMPLATE(struct, __VA_ARGS__)
#else
#define PXRUSDPREVIEWSURFACE_API                      ARCH_IMPORT
#define PXRUSDPREVIEWSURFACE_API_TEMPLATE_CLASS(...)  ARCH_IMPORT_TEMPLATE(class, __VA_ARGS__)
#define PXRUSDPREVIEWSURFACE_API_TEMPLATE_STRUCT(...) ARCH_IMPORT_TEMPLATE(struct, __VA_ARGS__)
#endif
#define PXRUSDPREVIEWSURFACE_LOCAL ARCH_HIDDEN
#endif
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="28">

---

# pxrUsdPreviewSurface Type Definitions

These lines define the type name, type ID, and registrant ID for the pxrUsdPreviewSurface plugin. These are used when initializing the plugin to ensure it is correctly registered with Maya.

```c++
namespace {
const MString  PxrUsdPreviewSurface_typeName("pxrUsdPreviewSurface");
const MTypeId  PxrUsdPreviewSurface_typeId(0x00126403);
const MString& PxrUsdPreviewSurface_registrantId = PxrUsdPreviewSurface_typeName;
} // namespace
```

---

</SwmSnippet>

# Functions of pxrUsdPreviewSurface

The 'pxrUsdPreviewSurface' is a key component in the 'maya-usd' repository. It is implemented across multiple files and contains several important functions and macros.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/api.h" line="21">

---

## PXRUSDPREVIEWSURFACE_API

The `PXRUSDPREVIEWSURFACE_API` macro is used to define the API for the 'pxrUsdPreviewSurface'. It is defined differently based on whether the 'PXR_STATIC' is defined or not.

```c
#if defined(PXR_STATIC)
#define PXRUSDPREVIEWSURFACE_API
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/api.h" line="23">

---

## PXRUSDPREVIEWSURFACE_API_TEMPLATE_CLASS

The `PXRUSDPREVIEWSURFACE_API_TEMPLATE_CLASS` macro is used to define a template class for the 'pxrUsdPreviewSurface' API.

```c
#define PXRUSDPREVIEWSURFACE_API_TEMPLATE_CLASS(...)
#define PXRUSDPREVIEWSURFACE_API_TEMPLATE_STRUCT(...)
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/api.h" line="24">

---

## PXRUSDPREVIEWSURFACE_API_TEMPLATE_STRUCT

The `PXRUSDPREVIEWSURFACE_API_TEMPLATE_STRUCT` macro is used to define a template structure for the 'pxrUsdPreviewSurface' API.

```c
#define PXRUSDPREVIEWSURFACE_API_TEMPLATE_STRUCT(...)
#define PXRUSDPREVIEWSURFACE_LOCAL
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/api.h" line="25">

---

## PXRUSDPREVIEWSURFACE_LOCAL

The `PXRUSDPREVIEWSURFACE_LOCAL` macro is used to define the local scope for the 'pxrUsdPreviewSurface'.

```c
#define PXRUSDPREVIEWSURFACE_LOCAL
#else
```

---

</SwmSnippet>

# pxrUsdPreviewSurface Plugin Endpoints

Understanding pxrUsdPreviewSurface Plugin Endpoints

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="44">

---

## initializePlugin

The `initializePlugin` function is an endpoint that is used to initialize the 'pxrUsdPreviewSurface' plugin. It registers the plugin with the Maya system and sets up necessary configurations.

```c++
MStatus initializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj, "Pixar", "1.0", "Any");

    status = PxrMayaUsdPreviewSurfacePlugin::initialize(
        plugin,
        PxrUsdPreviewSurface_typeName,
        PxrUsdPreviewSurface_typeId,
        PxrUsdPreviewSurface_registrantId);
    CHECK_MSTATUS(status);

    return MS::kSuccess;
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="60">

---

## uninitializePlugin

The `uninitializePlugin` function is another endpoint that is used to clean up when the 'pxrUsdPreviewSurface' plugin is no longer needed. It deregisters the plugin from the Maya system.

```c++
MStatus uninitializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj);

    status = PxrMayaUsdPreviewSurfacePlugin::finalize(
        plugin,
        PxrUsdPreviewSurface_typeName,
        PxrUsdPreviewSurface_typeId,
        PxrUsdPreviewSurface_registrantId);
    CHECK_MSTATUS(status);

    return MS::kSuccess;
}
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
