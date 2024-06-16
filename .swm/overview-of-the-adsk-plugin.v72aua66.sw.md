---
title: Overview of the adsk Plugin
---
The 'adsk Plugin' is a part of the Maya-USD project that provides Autodesk-specific implementations for the Universal Scene Description (USD) functionalities. It contains a collection of commands, translators, and nodes that are used to facilitate the import and export of USD data within Autodesk's Maya software. The plugin is organized into several directories, including 'plugin', 'scripts', and 'icons'. The 'plugin' directory contains the core implementation of the plugin, including various commands for importing and exporting USD data, and nodes for representing USD data within Maya. The 'scripts' directory contains MEL and Python scripts that are used to provide user interface elements and additional functionalities. The 'icons' directory contains icon files used within the plugin's user interface.

<SwmSnippet path="/plugin/adsk/plugin/adskImportCommand.h" line="23">

---

## ADSKMayaUSDImportCommand

The `ADSKMayaUSDImportCommand` class is a command that is used to import USD files into Maya. It is defined in the `adskImportCommand.h` file and is used in the `plugin.cpp` and `adskImportCommand.cpp` files.

```c
namespace MAYAUSD_NS_DEF {

class MAYAUSD_PLUGIN_PUBLIC ADSKMayaUSDImportCommand : public MayaUsd::MayaUSDImportCommand
{
public:
    static const MString commandName;

    static void* creator();
};
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/adsk/plugin/adskListShadingModesCommand.h" line="23">

---

## ADSKMayaUSDListShadingModesCommand

The `ADSKMayaUSDListShadingModesCommand` class is a command that lists the shading modes available in the Maya-USD plugin. It is defined in the `adskListShadingModesCommand.h` file.

```c
namespace MAYAUSD_NS_DEF {

class MAYAUSD_PLUGIN_PUBLIC ADSKMayaUSDListShadingModesCommand
    : public MayaUsd::MayaUSDListShadingModesCommand
{
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/adsk/plugin/adskMaterialCommands.h" line="31">

---

## ADSKMayaUSDGetMaterialsForRenderersCommand

The `ADSKMayaUSDGetMaterialsForRenderersCommand` class is a command that returns an array of materials associated with a given renderer. It is defined in the `adskMaterialCommands.h` file.

```c

//! \brief Returns an array of strings containing materials associated with a given renderer.The
//! strings are in the format: "Renderer Name/Material Label|MaterialIdentifier" e.g. "Arnold/AI
//! Standard Surface|arnold:standard_surface" The main intention is for the returned strings to be
//! split in order to populate menu entries. \todo: The list of materials and renderers is currently
//! hard-coded. We need to make it dynamic so that third-party renderers can hook in to provide
//! their own materials.
class MAYAUSD_PLUGIN_PUBLIC ADSKMayaUSDGetMaterialsForRenderersCommand : public MPxCommand
```

---

</SwmSnippet>

# Functions of adsk Plugin

The adsk Plugin contains several functions that are crucial for the operation of the plugin. These functions are spread across multiple files within the plugin/adsk/plugin directory.

<SwmSnippet path="/plugin/adsk/plugin/plugin.cpp" line="129">

---

## initializePlugin

The `initializePlugin` function is responsible for initializing the plugin. It registers the plugin with Autodesk's Maya software, sets up the necessary resources, and registers the necessary commands and translators.

```c++
MStatus initializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj, "Autodesk", TOSTRING(MAYAUSD_VERSION), "Any");

    // register string resources
    status = plugin.registerUIStrings(registerStringResources, "mayaUSDRegisterStrings");
    if (!status) {
        status.perror("mayaUsdPlugin: unable to register string resources.");
    }

    status = plugin.registerFileTranslator(
        "USD Import",
        "",
        UsdMayaImportTranslator::creator,
        "mayaUsdTranslatorImport", // options script name
        const_cast<char*>(UsdMayaImportTranslator::GetDefaultOptions().c_str()),
        false);
    if (!status) {
        status.perror("mayaUsdPlugin: unable to register import translator.");
    }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/adsk/plugin/plugin.cpp" line="297">

---

## uninitializePlugin

The `uninitializePlugin` function is responsible for cleaning up when the plugin is unloaded. It deregisters the commands and translators that were registered during initialization.

```c++
MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin plugin(obj);
    MStatus   status;

    status = PxrMayaUsdPreviewSurfacePlugin::finalize(
        plugin,
        kMayaUsdPreviewSurface_typeName,
        kMayaUsdPreviewSurface_typeId,
        kMayaUsdPreviewSurface_registrantId);
    CHECK_MSTATUS(status);

    status = UsdMayaUndoHelperCommand::finalize(plugin);
    if (!status) {
        status.perror(
            std::string("deregisterCommand ").append(UsdMayaUndoHelperCommand::name()).c_str());
    }

    deregisterCommandCheck<MayaUsd::ADSKMayaUSDListShadingModesCommand>(plugin);
    deregisterCommandCheck<MayaUsd::ADSKMayaUSDListJobContextsCommand>(plugin);

```

---

</SwmSnippet>

<SwmSnippet path="/plugin/adsk/plugin/adskExportCommand.h" line="25">

---

## ADSKMayaUSDExportCommand

The `ADSKMayaUSDExportCommand` class is a command that handles exporting USD data from Maya. It is registered and deregistered in the `initializePlugin` and `uninitializePlugin` functions respectively.

```c
class MAYAUSD_PLUGIN_PUBLIC ADSKMayaUSDExportCommand : public MayaUsd::MayaUSDExportCommand
{
public:
    static const MString commandName;

    static void* creator();
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/adsk/plugin/adskImportCommand.h" line="25">

---

## ADSKMayaUSDImportCommand

The `ADSKMayaUSDImportCommand` class is a command that handles importing USD data into Maya. It is registered and deregistered in the `initializePlugin` and `uninitializePlugin` functions respectively.

```c
class MAYAUSD_PLUGIN_PUBLIC ADSKMayaUSDImportCommand : public MayaUsd::MayaUSDImportCommand
{
public:
    static const MString commandName;

    static void* creator();
};
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/adsk/plugin/adskMaterialCommands.cpp" line="224">

---

## isNodeTypeInList

The `isNodeTypeInList` function is a utility function used to check if a given node type is in a list of node types. It is used in the `doIt` method of the `ADSKMayaUSDMaterialBindingsCommand` class.

```c++
static bool isNodeTypeInList(
    const Ufe::SceneItem::Ptr&      sceneItem,
    const std::vector<std::string>& nodeTypeList,
    bool                            checkAllAncestors)
{
    const auto canonicalName
        = TfType::Find<UsdSchemaBase>().FindDerivedByName(sceneItem->nodeType().c_str());

    if (canonicalName.IsUnknown()) {
        return false;
    }

    if (std::find(nodeTypeList.begin(), nodeTypeList.end(), canonicalName.GetTypeName().c_str())
        != nodeTypeList.end()) {
        // Our nodeType matches one in the given list.
        return true;
    }

    const auto& ancestors = sceneItem->ancestorNodeTypes();
    for (const auto& ancestorType : ancestors) {
        const auto canonicalAncestorName
```

---

</SwmSnippet>

# ProxyShape and ADSKMayaUSDExportCommand Explanation

Understanding ProxyShape and ADSKMayaUSDExportCommand

<SwmSnippet path="/plugin/adsk/plugin/ProxyShape.h" line="30">

---

## ProxyShape

The `ProxyShape` class is a custom Maya shape node that serves as the main access point to the USD data. It inherits from the `MayaUsdProxyShapeBase` class. The `ProxyShape` class defines several public methods such as `creator` and `initialize`, which are required for managing the lifecycle of the node within Maya.

```c
class ProxyShape : public MayaUsdProxyShapeBase
{
public:
    typedef MayaUsdProxyShapeBase ParentClass;

    MAYAUSD_PLUGIN_PUBLIC
    static const MTypeId typeId;
    MAYAUSD_PLUGIN_PUBLIC
    static const MString typeName;

    MAYAUSD_PLUGIN_PUBLIC
    static void* creator();

    MAYAUSD_PLUGIN_PUBLIC
    static MStatus initialize();

    void postConstructor() override;

private:
    ProxyShape();

```

---

</SwmSnippet>

<SwmSnippet path="/plugin/adsk/plugin/adskExportCommand.h" line="25">

---

## ADSKMayaUSDExportCommand

The `ADSKMayaUSDExportCommand` class is a custom command that is used to export Maya data to a USD file. It inherits from the `MayaUsd::MayaUSDExportCommand` class. The `ADSKMayaUSDExportCommand` class defines a `creator` method which is a factory function for creating instances of the command.

```c
class MAYAUSD_PLUGIN_PUBLIC ADSKMayaUSDExportCommand : public MayaUsd::MayaUSDExportCommand
{
public:
    static const MString commandName;

    static void* creator();
};
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
