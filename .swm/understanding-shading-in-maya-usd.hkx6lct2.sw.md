---
title: Understanding Shading in Maya-USD
---

Shading in the maya-usd repository refers to the process of creating a visual representation of how light interacts with a 3D object's surface in the Maya 3D software. This is achieved through the use of shaders, which are programs that define the color and brightness of pixels in a rendered image. The repository contains several files and functions dedicated to handling shading, such as the 'shadingModeRegistry.cpp' and 'shadingModeUseRegistry.cpp' files. These files contain functions for reading and writing shading data, as well as for registering different shading modes. Shading is a crucial part of the rendering process, as it gives objects a sense of depth and realism.

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeUseRegistry.cpp" line="445">

---

# Shading Import

The `UseRegistryShadingModeImporter` class is used to handle the import of shading data. The `Read` function is the main entry point for the import process, traversing a UsdMaterial to build a Maya shading network.

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

# Shading Export

The `Export` function is the main entry point for the export process. It handles the export of shading data, creating a UsdShadeMaterial from a Maya shading network.

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

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeRegistry.cpp" line="40">

---

# Shading Mode Registry

The `_ImportShadingMode` and `_ExportShadingMode` classes are used to handle the import and export of shading data respectively. They store information about the shading mode, including a nice name, a description, and a function to handle the import or export process.

```c++
struct _ExportShadingMode
{
    std::string                       _niceName;
    std::string                       _description;
    UsdMayaShadingModeExporterCreator _fn;
};
using _ExportRegistry = std::map<TfToken, _ExportShadingMode>;
static _ExportRegistry _exportReg;

TF_DEFINE_PUBLIC_TOKENS(UsdMayaPreferredMaterialTokens, PXRUSDMAYA_SHADINGCONVERSION_TOKENS);

using _MaterialConversionRegistry
    = std::unordered_map<TfToken, UsdMayaShadingModeRegistry::ConversionInfo, TfToken::HashFunctor>;
static _MaterialConversionRegistry _conversionReg;

bool UsdMayaShadingModeRegistry::RegisterExporter(
    const std::string&                name,
    std::string                       niceName,
    std::string                       description,
    UsdMayaShadingModeExporterCreator fn)
{
```

---

</SwmSnippet>

# Shading Functions

This section discusses the main functions related to shading in the 'maya-usd' repository.

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModePxrRis.cpp" line="89">

---

## \_GetShadingPlugs Function

The `_GetShadingPlugs` function is used to get shading plugs based on the preference set. It checks for the 'rfmShadingEngineUseRmanPlugs' preference and uses the 'rman**surface' and 'rman**displacement' plug names if set to 1. Otherwise, it falls back to Maya's 'surfaceShader' and 'displacementShader'.

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

The `_ExportShadingNodeHelper` function is used to export shading nodes. It takes the material prim, dependency node, context, and processed paths as arguments. It determines the 'risShaderType' that will correspond to the USD shader ID and creates a shader schema. It also gathers all the 'inputAttrPlugs' to process.

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

The `_ExportShadingNode` function is used to export shading nodes. It calls the `_ExportShadingNodeHelper` function to perform the actual export.

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

The `Export` function is the main function used to export shading. It creates a material prim, binds it, and then exports surface, volume, and displacement shaders by calling the `_ExportShadingNode` function.

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

# Shading Data Export and Import

Shading Export and Import in Maya-USD

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeExporter.h" line="42">

---

## UsdMayaShadingModeExporter::DoExport

The `DoExport` method in the `UsdMayaShadingModeExporter` class is responsible for exporting shading data from Maya to USD. It takes a `UsdMayaWriteJobContext` and a map of DAG paths to USD paths as arguments.

```c
    void DoExport(
        UsdMayaWriteJobContext&                  writeJobContext,
        const UsdMayaUtil::MDagPathMap<SdfPath>& dagPathToUsdMap);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeRegistry.h" line="75">

---

## UsdMayaShadingModeRegistry::GetExporter

The `GetExporter` function in the `UsdMayaShadingModeRegistry` class is used to retrieve a shading mode exporter based on its name. This allows for different shading modes to be used during the export process.

```c
    static UsdMayaShadingModeExporterCreator GetExporter(const TfToken& name)
    {
        return GetInstance()._GetExporter(name);
    }
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
