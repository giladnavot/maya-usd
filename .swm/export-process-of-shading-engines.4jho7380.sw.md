---
title: Export Process of Shading Engines
---
This document will cover the process of exporting shading engines in the Maya-USD plugin. The process includes the following steps:

1. Initiating the export process
2. Exporting the shading mode
3. Creating a standard material prim
4. Binding the standard material prim
5. Assigning component tags
6. Getting the attribute adaptor

```mermaid
graph TD;
subgraph lib/mayaUsd/fileio/shading
  ExportShadingEngines:::mainFlowStyle --> DoExport
end
subgraph lib/mayaUsd/fileio/shading
  DoExport:::mainFlowStyle --> Export
end
subgraph lib/mayaUsd/fileio/shading
  Export:::mainFlowStyle --> MakeStandardMaterialPrim
end
subgraph lib/mayaUsd/fileio/shading
  Export:::mainFlowStyle --> BindStandardMaterialPrim
end
subgraph lib/mayaUsd/fileio/shading
  BindStandardMaterialPrim:::mainFlowStyle --> _AssignComponentTags
end
subgraph lib/mayaUsd/fileio
  _AssignComponentTags:::mainFlowStyle --> Get
end
  Get:::mainFlowStyle --> ...

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorMaterial.cpp" line="1">

---

# Initiating the export process

The process begins with the `ExportShadingEngines` function in `translatorMaterial.cpp`. This function is responsible for initiating the export process of shading engines.

```c++
//
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeExporter.cpp" line="81">

---

# Exporting the shading mode

The `DoExport` function in `shadingModeExporter.cpp` is called next. This function handles the export of the shading mode. It prepares the context for the export, performs pre-export operations, exports the shading engines, and finally performs post-export operations.

```c++
void UsdMayaShadingModeExporter::DoExport(
    UsdMayaWriteJobContext&                  writeJobContext,
    const UsdMayaUtil::MDagPathMap<SdfPath>& dagPathToUsdMap)
{
    MayaUsd::ProgressBarScope progressBar(4);

    const UsdMayaJobExportArgs& exportArgs = writeJobContext.GetArgs();
    const UsdStageRefPtr&       stage = writeJobContext.GetUsdStage();

    const SdfPath& materialCollectionsPath = exportArgs.exportMaterialCollections
        ? exportArgs.materialCollectionsPath
        : SdfPath::EmptyPath();

    UsdPrim materialCollectionsPrim;
    if (!materialCollectionsPath.IsEmpty()) {
        materialCollectionsPrim = stage->OverridePrim(materialCollectionsPath);
        if (!materialCollectionsPrim) {
            TF_WARN(
                "Error: could not override prim at path <%s>. One of the "
                "ancestors of the path must be inactive or an instance root. "
                "Not exporting material collections!",
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModePxrRis.cpp" line="271">

---

# Creating a standard material prim

The `Export` function in `shadingModePxrRis.cpp` is then called. This function creates a standard material prim and binds it.

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

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeExporterContext.cpp" line="559">

---

# Binding the standard material prim

The `MakeStandardMaterialPrim` function in `shadingModeExporterContext.cpp` is responsible for creating a standard material prim and returning it.

```c++
UsdPrim UsdMayaShadingModeExportContext::MakeStandardMaterialPrim(
    const AssignmentsInfo& assignmentsInfo,
    const std::string&     name) const
{
    const UsdMayaJobExportArgs& exportArgs = GetExportArgs();

    if (!shouldExportMaterial(assignmentsInfo, GetSurfaceShader(), exportArgs))
        return UsdPrim();

    const std::string materialName
        = UsdUfe::sanitizeName(getMaterialName(name, _shadingEngine, GetSurfaceShader()));
    if (materialName.empty())
        return UsdPrim();

    UsdStageRefPtr stage = GetUsdStage();
    UsdPrim        materialParent = exportArgs.legacyMaterialScope
        ? _GetLegacyMaterialParent(
            stage, exportArgs.materialsScopeName, assignmentsInfo.assignments)
        : _GetMaterialParent(stage, exportArgs.defaultPrim, exportArgs.materialsScopeName);
    if (!materialParent)
        return UsdPrim();
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeExporterContext.cpp" line="844">

---

# Assigning component tags

The `_AssignComponentTags` function in `shadingModeExporterContext.cpp` is called next. This function assigns component tags to the material prim.

```c++
/// We might have one or more component tags that have roundtrip data that covers this
// set of faces. Assign them to those tags and return the remaining unhandled faces.
VtIntArray _AssignComponentTags(
    const UsdMayaShadingModeExportContext* ctx,
    const UsdShadeMaterial&                materialToBind,
    const MObjectHandle&                   geomHandle,
    const std::vector<UsdGeomSubset>&      currentGeomSubsets,
    const VtIntArray&                      faceIndices,
    SdfPathSet* const                      boundPrimPaths)
{
    if (currentGeomSubsets.empty() || faceIndices.empty() || !geomHandle.isValid()) {
        return faceIndices;
    }

    JsValue info;
    if (UsdMayaMeshReadUtils::getGeomSubsetInfo(geomHandle.object(), info) && info) {
        auto subsetInfoDict = info.GetJsObject();

        std::set<VtIntArray::ElementType> faceSet, handledSet;
        faceSet.insert(faceIndices.cbegin(), faceIndices.cend());

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/adaptor.cpp" line="1041">

---

# Getting the attribute adaptor

Finally, the `Get` function in `adaptor.cpp` is called. This function retrieves the attribute adaptor for the material prim.

```c++
bool UsdMayaAttributeAdaptor::Get(VtValue* value) const
{
    if (!*this) {
        return false;
    }

#if PXR_VERSION < 2308
    VtValue result = UsdMayaWriteUtil::GetVtValue(_plug, _attrDef->GetTypeName());
#else
    VtValue result = UsdMayaWriteUtil::GetVtValue(_plug, _attrDef.GetTypeName());
#endif
    if (result.IsEmpty()) {
        return false;
    }

    *value = result;
    return true;
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="flows"><sup>Powered by [Swimm](/)</sup></SwmMeta>
