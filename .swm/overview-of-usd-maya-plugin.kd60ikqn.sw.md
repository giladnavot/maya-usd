---
title: Overview of USD Maya Plugin
---
The USD Maya Plugin, also known as 'pxrUsd', is a plugin that integrates Pixar's Universal Scene Description (USD) functionality into Maya. It provides import and export commands, as well as USD nodes such as Pixar's USD reference assembly and the Pixar-specific USD proxy shape. The plugin is responsible for registering these functionalities and nodes in Maya, as well as handling their initialization and uninitialization. It also includes commands for exporting and importing USD files, and for listing shading modes. The majority of the functionality has been absorbed into the core mayaUsd library, and the 'mayaUsdPlugin' should be favored over 'pxrUsd'.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="47">

---

# Plugin Initialization

This section of the code shows how the plugin is initialized when Maya starts up. It registers the plugin with Maya, initializes various components of the plugin, and registers various commands and nodes that the plugin provides.

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

# Plugin Uninitialization

This section of the code shows how the plugin is un-initialized when Maya shuts down. It deregisters the plugin from Maya, finalizes various components of the plugin, and deregisters various commands and nodes that the plugin provides.

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

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugInfo.json" line="1">

---

# Plugin Information

This JSON file provides information about the plugin, including the name of the plugin and the types of nodes that it provides.

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

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/overview.dox" line="1">

---

# Plugin Overview

This document provides an overview of the plugin, including its purpose and the functionality that it provides.

```dox
/*!
\page usd_maya_plugin_pxrUsd_page_front PxrUsd : Pixar's USD plugin for Maya
\if ( PIXAR_MFB_BUILD )
\mainpage PxrUsd : Pixar's USD plugin for Maya
\publicLib
\endif

## Overview

This is the plugin that registers Pixar's USD functionality in Maya, including
import and export commands, as well as USD nodes such as Pixar's USD reference
assembly and the Pixar-specific USD proxy shape.

The majority of the functionality has been absorbed into the core mayaUsd
library, and the "mayaUsdPlugin" should be favored over "pxrUsd". What remains
in the Pixar plugin is implemented in the
\ref usd_maya_lib_usdMaya_page_front "UsdMaya library".

*/
```

---

</SwmSnippet>

# USD Maya Plugin Functions

The USD Maya Plugin provides a set of functionalities that are implemented across various files and directories in the repository.

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/plugin.cpp" line="46">

---

## initializePlugin

The `initializePlugin` function is responsible for initializing the plugin in Maya. It sets up the plugin, initializes the UFE (Universal Front End), registers the proxy shape plugin, and sets up the assembly node type for the scene assembly menu in Maya.

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

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/alembicChaser.cpp" line="253">

---

## AlembicChaser

The `AlembicChaser` class is an example of a chaser plugin provided by the pxr plugin. It supports `attrprefix` and `primvarprefix` to export custom attributes and primvars. It is used to make integrating USD into Alembic-heavy pipelines easier.

```c++
// this plugin is provided as an example and can be updated to more closely
// match what exporting a file from maya to alembic does.  For now, it just
// supports attrprefix and primvarprefix to export custom attributes and
// primvars.
class AlembicChaser : public UsdMayaExportChaser
{
public:
    AlembicChaser(
        UsdStagePtr                                                     stage,
        const UsdMayaExportChaserRegistry::FactoryContext::DagToUsdMap& dagToUsd,
        const std::map<std::string, std::string>&                       attrPrefixes,
        const std::map<std::string, std::string>&                       primvarPrefixes)
        : _stage(stage)
        , _dagToUsd(dagToUsd)
    {
        for (const auto& p : dagToUsd) {
            const MDagPath& dag = p.first;
            const SdfPath&  usdPrimPath = p.second;

            UsdPrim usdPrim = stage->GetPrimAtPath(usdPrimPath);
            if (!dag.isValid() || !usdPrim) {
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsd/alembicChaser.cpp" line="337">

---

## PXRUSDMAYA_DEFINE_EXPORT_CHASER_FACTORY

The `PXRUSDMAYA_DEFINE_EXPORT_CHASER_FACTORY` function is used to define a factory for creating an instance of the AlembicChaser. It parses the arguments passed to it and creates a new instance of AlembicChaser with the parsed arguments.

```c++
PXRUSDMAYA_DEFINE_EXPORT_CHASER_FACTORY(alembic, ctx)
{
    std::map<std::string, std::string> myArgs;
    TfMapLookup(ctx.GetJobArgs().allChaserArgs, "alembic", &myArgs);

    // The attrprefix and primvarprefix arguments provide the
    // prefixes for attributes/primvars to export from Maya and an optional
    // replacement prefix for the output USD attribute name.
    //
    // The format of the argument is:
    //    mayaPrefix1[=usdPrefix1],mayaPrefix2[=usdPrefix2],...
    // usdPrefix can contain namespaces (i.e. can have colons) for attrprefix
    // but not for primvarprefix (since primvar names can't have namespaces).
    // If [=usdPrefix] is omitted, then these defaults are used:
    //    - for attrprefix, "userProperties:"
    //    - for primvarprefix, "" (empty)
    //
    // Example attrprefix:
    //    ABC_,ABC2_=customPrefix_,ABC3_=,ABC4_=customNamespace:
    // ABC_attrName  -> userProperties:attrName
    // ABC2_attrName -> customPrefix_attrName
```

---

</SwmSnippet>

# USD Maya Plugin Endpoints

Understanding the USD Maya Plugin Endpoints

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="44">

---

## initializePlugin

The `initializePlugin` function is an endpoint that is called when the plugin is loaded into Maya. It is responsible for initializing the plugin, registering the shader reader and writer, and setting up the plugin for use.

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

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="59">

---

## uninitializePlugin

The `uninitializePlugin` function is an endpoint that is called when the plugin is unloaded from Maya. It is responsible for cleaning up and deregistering the plugin.

```c++
PXRUSDPREVIEWSURFACE_API
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

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
