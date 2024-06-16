---
title: Exploring the Autodesk Plugin
---

The 'adsk Plugin' is a part of the Maya-USD plugin system that provides Autodesk-specific implementations for the Universal Scene Description (USD) functionalities. It contains a collection of header and source files that define various commands, translators, and proxy shapes. The plugin also includes scripts and icons used within the Autodesk Maya environment. The 'MAYAUSD_PLUGIN_PUBLIC' macro is frequently used in this plugin's codebase, which is a directive for symbol visibility in shared libraries.

<SwmSnippet path="/plugin/adsk/plugin/adskImportCommand.h" line="23">

---

## ADSKMayaUSDImportCommand

The `ADSKMayaUSDImportCommand` class is a command that allows for the importation of USD files into Maya. It is defined in the `adskImportCommand.h` file and is used in the `plugin.cpp` and `adskImportCommand.cpp` files.

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

The `ADSKMayaUSDListShadingModesCommand` class is a command that lists the shading modes available in Maya. It extends the `MayaUSDListShadingModesCommand` class.

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

The `ADSKMayaUSDGetMaterialsForRenderersCommand` class is a command that returns an array of materials associated with a given renderer in Maya. It is defined in the `adskMaterialCommands.h` file.

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

This section provides an overview of the main functions in the adsk Plugin.

<SwmSnippet path="/plugin/adsk/plugin/plugin.cpp" line="126">

---

## initializePlugin

The `initializePlugin` function is responsible for initializing the plugin. It registers string resources, file translators, and various commands. It also checks for the availability of certain features and registers additional commands if they are available.

```c++
};

MAYAUSD_PLUGIN_PUBLIC
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
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/adsk/plugin/plugin.cpp" line="297">

---

## uninitializePlugin

The `uninitializePlugin` function is responsible for cleaning up when the plugin is unloaded. It deregisters file translators and commands that were registered during initialization.

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

The `ADSKMayaUSDExportCommand` class is a command for exporting USD. It is registered and deregistered in the `initializePlugin` and `uninitializePlugin` functions respectively.

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

The `ADSKMayaUSDImportCommand` class is a command for importing USD. Similar to the `ADSKMayaUSDExportCommand`, it is registered and deregistered in the `initializePlugin` and `uninitializePlugin` functions respectively.

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

# adsk Plugin Endpoints

Understanding the adsk Plugin Endpoints

<SwmSnippet path="/plugin/adsk/scripts/AETemplateHelpers.py" line="20">

---

## GetAllRootPrimNamesNaturalOrder

The `GetAllRootPrimNamesNaturalOrder` function is used to get all root primitive names in a natural order from a given proxy shape. It first gets the USD stage associated with the proxy shape, then it retrieves all the primitive names and sorts them in a natural order.

```python
def GetAllRootPrimNamesNaturalOrder(proxyShape):
    # Custom comparator key
    def natural_key(item):
        return [int(s) if s.isdigit() else s.lower() for s in __naturalOrderRE.split(item)]
    try:
        proxyStage = mayaUsd.ufe.getStage(proxyShape)
        primNames = []
        if proxyStage:
            for prim in proxyStage.TraverseAll():
                if (prim.GetPath().IsRootPrimPath()):
                    primNames.append(prim.GetName())
        # Sort the prim list in natural order
        primNames.sort(key=natural_key)
        return primNames
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/adsk/scripts/AETemplateHelpers.py" line="51">

---

## SetDefaultPrim

The `SetDefaultPrim` function is used to set a default primitive for a given proxy shape. It first gets the USD stage associated with the proxy shape, then it sets the specified primitive name as the default primitive. If the operation fails, it displays an error message.

```python
def SetDefaultPrim(proxyShape, primName):
    try:
        proxyStage = mayaUsd.ufe.getStage(proxyShape)
        if not proxyStage:
            return False

        cmd = None
        if not primName:
            cmd = usdUfe.ClearDefaultPrimCommand(proxyStage)
        else:
            defaultPrim = proxyStage.GetPrimAtPath('/' + primName)
            if defaultPrim:
                cmd = usdUfe.SetDefaultPrimCommand(defaultPrim)

        if cmd is None:
            return False

        ufeCmd.execute(cmd)
        return True
    except Exception as e:
        # Note: we do want to tell the user why the set or clear failed.
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
