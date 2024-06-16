---
title: Exploring Shading in Maya-USD
---
Shading in the maya-usd repository refers to the process of creating a Maya shading network that reproduces the information found in the USD shading network. This is achieved through a series of functions and classes that handle the import and export of shading data, the creation of shaders, and the connection of shading attributes. The shading process involves the use of shading modes, which are registered and handled through the ShadingModeRegistry. Shading modes define how shading networks are imported and exported between Maya and USD.

The Read function in the ShadingModeUseRegistry is a key part of the shading process. It traverses a UsdMaterial to build a Maya shading network. It handles different types of shaders including surface, volume, and displacement shaders. It also creates a shading engine, which is a Maya node that represents a material.

The ShadingModeRegistry maintains a registry of shading modes for both import and export. Each shading mode has a nice name, a description, and a function that creates a shader writer or reader. The shader writer or reader is responsible for translating the shading data between Maya and USD.

The \_ExportShadingDepGraph function is another key part of the shading process. It exports nodes in the Maya dependency graph under a material export path. The first shader prim authored during the traversal is considered the primary shader for the connection.

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeRegistry.cpp" line="40">

---

# Shading Mode Registry

The `_ExportShadingMode` struct is used to store information about a shading mode during export. It includes a nice name, a description, and a function to create the exporter.

```c++
struct _ExportShadingMode
{
    std::string                       _niceName;
    std::string                       _description;
    UsdMayaShadingModeExporterCreator _fn;
};
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeRegistry.cpp" line="97">

---

Similarly, the `_ImportShadingMode` struct is used to store information about a shading mode during import. It also includes a nice name, a description, and a function to create the importer.

```c++
struct _ImportShadingMode
{
    std::string                _niceName;
    std::string                _description;
    UsdMayaShadingModeImporter _fn;
};
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeUseRegistry.cpp" line="445">

---

# Shading Mode Importer

The `UseRegistryShadingModeImporter` class is used to import shading networks from USD to Maya. It traverses a UsdMaterial and builds a Maya shading network that reproduces the information found in the USD shading network.

```c++
/// This class implements a shading mode importer which uses a registry keyed by the info:id USD
/// attribute to provide an importer class for each UsdShade node processed while traversing the
/// main connections of a UsdMaterial node.
class UseRegistryShadingModeImporter
{
public:
    UseRegistryShadingModeImporter(
        UsdMayaShadingModeImportContext* context,
        const UsdMayaJobImportArgs&      jobArguments)
        : _context(context)
        , _jobArguments(jobArguments)
    {
    }

    /// Main entry point of the import process. On input we get a UsdMaterial which gets traversed
    /// in order to build a Maya shading network that reproduces the information found in the USD
    /// shading network.
    MObject Read()
    {
        if (_jobArguments.shadingModes.size() != 1) {
            // The material translator will make sure we only get a single shading mode
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeUseRegistry.cpp" line="344">

---

# Shading Mode Exporter

The `Export` function is used to export shading networks from Maya to USD. It traverses a Maya shading network and builds a UsdMaterial that reproduces the information found in the Maya shading network.

```c++
    void Export(
        const UsdMayaShadingModeExportContext& context,
        UsdShadeMaterial* const                mat,
        SdfPathSet* const                      boundPrimPaths) override
    {
        if (!context.GetExportArgs().exportMaterials) {
            return;
        }

        if (context.GetExportArgs().allMaterialConversions.empty()) {
            return;
        }

        MStatus status;

        MObject                 shadingEngine = context.GetShadingEngine();
        const MFnDependencyNode shadingEngineDepNodeFn(shadingEngine, &status);
        if (status != MS::kSuccess) {
            TF_RUNTIME_ERROR(
                "Cannot export invalid shading engine node '%s'\n",
                UsdMayaUtil::GetMayaNodeName(shadingEngine).c_str());
```

---

</SwmSnippet>

# Shading Functions

This section covers the main functions related to shading in the Maya-USD plugin.

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModePxrRis.cpp" line="89">

---

## \_GetShadingPlugs Function

The `_GetShadingPlugs` function is used to check for the rfmShadingEngineUseRmanPlugs preference. If set to 1, it uses rman__surface and rman__displacement plug names. Otherwise, it falls back to Maya's surfaceShader and displacementShader.

```c++
static _ShadingPlugs _GetShadingPlugs()
{
    // Check for rfmShadingEngineUseRmanPlugs preference
    // If set to 1, use rman__surface and rman__displacement plug names
    // Otherwise, fallback to Maya's surfaceShader and displacementShader
    bool exists = false;
    int  useRmanPlugs
        = MGlobal::optionVarIntValue(_tokens->RmanPlugPreferenceName.GetText(), &exists);
    return (exists && useRmanPlugs) ? _RmanPlugs : _MayaPlugs;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModePxrRis.cpp" line="131">

---

## \_ExportShadingNodeHelper Function

The `_ExportShadingNodeHelper` function is used to export shading nodes. It determines the risShaderType that will correspond to the USD shader ID and gathers all the inputAttrPlugs to process.

```c++
    UsdPrim _ExportShadingNodeHelper(
        const UsdPrim&                         materialPrim,
        const MFnDependencyNode&               depNode,
        const UsdMayaShadingModeExportContext& context,
        SdfPathSet*                            processedPaths)
    {
        UsdStagePtr stage = materialPrim.GetStage();

        // XXX: would be nice to write out the current display color as
        // well.  currently, when we re-import, we don't get the display color so
        // it shows up as black.

        const TfToken shaderPrimName(UsdUfe::sanitizeName(depNode.name().asChar()));
        const SdfPath shaderPath = materialPrim.GetPath().AppendChild(shaderPrimName);
        if (processedPaths->count(shaderPath) == 1u) {
            return stage->GetPrimAtPath(shaderPath);
        }

        processedPaths->insert(shaderPath);

        // Determine the risShaderType that will correspond to the USD shader ID.
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModePxrRis.cpp" line="262">

---

## \_ExportShadingNode Function

The `_ExportShadingNode` function is used to export shading nodes. It calls the `_ExportShadingNodeHelper` function to do the actual work.

```c++
    UsdPrim _ExportShadingNode(
        const UsdPrim&                         materialPrim,
        const MFnDependencyNode&               depNode,
        const UsdMayaShadingModeExportContext& context)
    {
        SdfPathSet processedNodes;
        return _ExportShadingNodeHelper(materialPrim, depNode, context, &processedNodes);
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModePxrRis.cpp" line="271">

---

## Export Function

The `Export` function is the main entry point for exporting shading information. It creates and binds a material prim, and exports surface, volume, and displacement shaders.

```c++
    void Export(
        const UsdMayaShadingModeExportContext& context,
        UsdShadeMaterial* const                mat,
        SdfPathSet* const                      boundPrimPaths) override
    {
        const UsdMayaShadingModeExportContext::AssignmentsInfo& assignments
            = context.GetAssignments();

        UsdPrim materialPrim = context.MakeStandardMaterialPrim(assignments);
        context.BindStandardMaterialPrim(materialPrim, assignments.assignments, boundPrimPaths);
        UsdShadeMaterial material(materialPrim);
        if (!material) {
            return;
        }

        if (mat != nullptr) {
            *mat = material;
        }

        UsdRiMaterialAPI riMaterialAPI(materialPrim);

```

---

</SwmSnippet>

# Shading Mode Registry Functions

Shading Mode Registry

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeRegistry.h" line="75">

---

## UsdMayaShadingModeRegistry::GetExporter

The `GetExporter` function is a static method of the `UsdMayaShadingModeRegistry` class. It takes a `TfToken` as an argument, which represents the name of the exporter. This function is used to retrieve a shading mode exporter based on its name.

```c
    static UsdMayaShadingModeExporterCreator GetExporter(const TfToken& name)
    {
        return GetInstance()._GetExporter(name);
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeRegistry.h" line="79">

---

## UsdMayaShadingModeRegistry::GetImporter

The `GetImporter` function is another static method of the `UsdMayaShadingModeRegistry` class. Similar to `GetExporter`, it takes a `TfToken` as an argument representing the name of the importer. This function is used to retrieve a shading mode importer based on its name.

```c
    static UsdMayaShadingModeImporter GetImporter(const TfToken& name)
    {
        return GetInstance()._GetImporter(name);
    }
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
