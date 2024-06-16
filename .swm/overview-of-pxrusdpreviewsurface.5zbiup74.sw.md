---
title: Overview of pxrUsdPreviewSurface
---

The `pxrUsdPreviewSurface` is a component of the Maya plugin in the maya-usd repository. It is defined and used within the `plugin.cpp` and `api.h` files in the `plugin/pxr/maya/plugin/pxrUsdPreviewSurface/` directory. The `pxrUsdPreviewSurface` is initialized in the `initializePlugin` function, which is marked with the `PXRUSDPREVIEWSURFACE_API` macro. This macro is defined in the `api.h` file and it controls the export and import of the `pxrUsdPreviewSurface` depending on whether it is being compiled as a static or dynamic library. The `pxrUsdPreviewSurface` is identified by a unique type name and type ID, which are defined as `PxrUsdPreviewSurface_typeName` and `PxrUsdPreviewSurface_typeId` respectively.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="43">

---

# Initialization of the Plugin

Here, the 'pxrUsdPreviewSurface' is used in the 'initializePlugin' function. The function takes in an object and uses the 'pxrUsdPreviewSurface' to initialize the plugin with the necessary parameters.

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

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="28">

---

# Definition of PxrUsdPreviewSurface

This is where the 'PxrUsdPreviewSurface_typeName' is defined as 'pxrUsdPreviewSurface'. It is then used as a parameter in the 'initializePlugin' function.

```c++
namespace {
const MString  PxrUsdPreviewSurface_typeName("pxrUsdPreviewSurface");
const MTypeId  PxrUsdPreviewSurface_typeId(0x00126403);
const MString& PxrUsdPreviewSurface_registrantId = PxrUsdPreviewSurface_typeName;
```

---

</SwmSnippet>

# Functions of pxrUsdPreviewSurface

The 'pxrUsdPreviewSurface' is a key component in the 'maya-usd' repository. It is implemented in several files, and it includes several important functions and variables.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="29">

---

## PxrUsdPreviewSurface_typeName

The `PxrUsdPreviewSurface_typeName` is a constant string that holds the name of the 'pxrUsdPreviewSurface' type.

```c++
const MString  PxrUsdPreviewSurface_typeName("pxrUsdPreviewSurface");
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="30">

---

## PxrUsdPreviewSurface_typeId

The `PxrUsdPreviewSurface_typeId` is a constant that holds the ID of the 'pxrUsdPreviewSurface' type.

```c++
const MTypeId  PxrUsdPreviewSurface_typeId(0x00126403);
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="31">

---

## PxrUsdPreviewSurface_registrantId

The `PxrUsdPreviewSurface_registrantId` is a constant string that holds the registrant ID of the 'pxrUsdPreviewSurface' type.

```c++
const MString& PxrUsdPreviewSurface_registrantId = PxrUsdPreviewSurface_typeName;
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="44">

---

## initializePlugin

The `initializePlugin` function is used to initialize the 'pxrUsdPreviewSurface' plugin. It takes an object as a parameter and returns a status.

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

# pxrUsdPreviewSurface Plugin Endpoints

Understanding pxrUsdPreviewSurface Plugin

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="44">

---

## initializePlugin

The `initializePlugin` function is an endpoint that is called when the plugin is loaded into the Maya environment. It registers the plugin with Maya, sets up necessary configurations, and prepares the plugin for use.

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

The `uninitializePlugin` function is an endpoint that is called when the plugin is unloaded from the Maya environment. It performs cleanup tasks and deregisters the plugin from Maya.

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
