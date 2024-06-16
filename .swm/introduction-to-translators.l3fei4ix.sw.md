---
title: Introduction to Translators
---
Translators in the maya-usd repository refer to a set of functionalities that allow the conversion of data between different formats or structures. They are primarily used for the import and export of geometry data, such as meshes and curves, between Maya and USD formats. The translators are organized into namespaces, such as GeometryExportOptions and GeometryImportOptions, each containing options for specific types of data conversion. For instance, options for exporting mesh geometry, mesh points, mesh normals, and more are defined in the GeometryExportOptions namespace. Similarly, options for importing meshes and curves are defined in the GeometryImportOptions namespace.

<SwmSnippet path="/plugin/al/translators/CommonTranslatorOptions.h" line="6">

---

# Translators Namespace

The 'translators' namespace is where all the translator functions and options are defined. This is the main entry point for using translators in the codebase.

```c
namespace translators {

//----------------------------------------------------------------------------------------------------------------------
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/translators/CommonTranslatorOptions.h" line="9">

---

# Geometry Export Options

Within the 'translators' namespace, there are sub-namespaces for specific types of data. For example, the 'GeometryExportOptions' namespace contains options for exporting various types of geometry data from Maya to USD.

```c
namespace GeometryExportOptions {
static constexpr const char* const kMeshes = "Meshes"; ///< export mesh geometry option name
static constexpr const char* const kMeshConnects
    = "Mesh Face Connects";                                       ///< export mesh face connects
static constexpr const char* const kMeshPoints = "Mesh Points";   ///< export mesh points
static constexpr const char* const kMeshExtents = "Mesh Extents"; ///< export mesh extents
static constexpr const char* const kMeshNormals = "Mesh Normals"; ///< export mesh normals
static constexpr const char* const kMeshVertexCreases
    = "Mesh Vertex Creases"; ///< export mesh vertex creases
static constexpr const char* const kMeshEdgeCreases
    = "Mesh Edge Creases";                                       ///< export mesh edge creases
static constexpr const char* const kMeshUvs = "Mesh UVs";        ///< export mesh UV coordinates
static constexpr const char* const kMeshUvOnly = "Mesh UV Only"; ///< export mesh UV coordinates
static constexpr const char* const kMeshPointsAsPref
    = "Mesh Points as PRef"; ///< export mesh Points as PRef, duplicating "P"
static constexpr const char* const kMeshColours = "Mesh Colours"; ///< export mesh Colour Sets
static constexpr const char* const kMeshDefaultColourRGB = "Default RGB"; ///< default rgb values
static constexpr const char* const kMeshDefaultColourA = "Default Alpha"; ///< default alpha values
static constexpr const char* const kCustomColourThreshold
    = "Custom Colour Threshold"; ///< if true, allow setting custom color threshold value
static constexpr const char* const kColourThresholdValue
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/translators/CommonTranslatorOptions.h" line="46">

---

# Geometry Import Options

Similarly, the 'GeometryImportOptions' namespace contains options for importing geometry data from USD to Maya. This allows for fine-grained control over how data is converted between the two formats.

```c
namespace GeometryImportOptions {
static constexpr const char* const kMeshes = "Import Meshes"; ///< the import meshes option name
static constexpr const char* const kNurbsCurves
    = "Import Curves"; ///< the import curves option name
} // namespace GeometryImportOptions
```

---

</SwmSnippet>

# Translators Functions

This section will cover the main functions of the translators in the 'maya-usd' repository.

<SwmSnippet path="/plugin/al/translators/CommonTranslatorOptions.h" line="9">

---

## GeometryExportOptions and GeometryImportOptions

The 'GeometryExportOptions' and 'GeometryImportOptions' namespaces define a set of options for exporting and importing geometry data. These options include settings for exporting and importing meshes, nurbs curves, and other types of geometry. Each option is represented by a static constant character string.

```c
namespace GeometryExportOptions {
static constexpr const char* const kMeshes = "Meshes"; ///< export mesh geometry option name
static constexpr const char* const kMeshConnects
    = "Mesh Face Connects";                                       ///< export mesh face connects
static constexpr const char* const kMeshPoints = "Mesh Points";   ///< export mesh points
static constexpr const char* const kMeshExtents = "Mesh Extents"; ///< export mesh extents
static constexpr const char* const kMeshNormals = "Mesh Normals"; ///< export mesh normals
static constexpr const char* const kMeshVertexCreases
    = "Mesh Vertex Creases"; ///< export mesh vertex creases
static constexpr const char* const kMeshEdgeCreases
    = "Mesh Edge Creases";                                       ///< export mesh edge creases
static constexpr const char* const kMeshUvs = "Mesh UVs";        ///< export mesh UV coordinates
static constexpr const char* const kMeshUvOnly = "Mesh UV Only"; ///< export mesh UV coordinates
static constexpr const char* const kMeshPointsAsPref
    = "Mesh Points as PRef"; ///< export mesh Points as PRef, duplicating "P"
static constexpr const char* const kMeshColours = "Mesh Colours"; ///< export mesh Colour Sets
static constexpr const char* const kMeshDefaultColourRGB = "Default RGB"; ///< default rgb values
static constexpr const char* const kMeshDefaultColourA = "Default Alpha"; ///< default alpha values
static constexpr const char* const kCustomColourThreshold
    = "Custom Colour Threshold"; ///< if true, allow setting custom color threshold value
static constexpr const char* const kColourThresholdValue
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/translators/CommonTranslatorOptions.cpp" line="31">

---

## registerCommonTranslatorOptions

The 'registerCommonTranslatorOptions' function is used to register the options defined in the 'GeometryExportOptions' and 'GeometryImportOptions' namespaces. It creates a new 'PluginTranslatorOptions' object for each set of options and adds the options to it. This function is called when the plugin is loaded to set up the options for the translators.

```c++
void registerCommonTranslatorOptions()
{
    auto context = AL::maya::utils::PluginTranslatorOptionsContextManager::find("ExportTranslator");
    if (context) {
        g_exportOptions = new AL::maya::utils::PluginTranslatorOptions(*context, "Geometry Export");
        g_exportOptions->addBool(GeometryExportOptions::kNurbsCurves, true);
        g_exportOptions->addBool(GeometryExportOptions::kMeshes, true);
        g_exportOptions->addBool(GeometryExportOptions::kMeshConnects, true);
        g_exportOptions->addBool(GeometryExportOptions::kMeshPoints, true);
        g_exportOptions->addBool(GeometryExportOptions::kMeshExtents, true);
        g_exportOptions->addBool(GeometryExportOptions::kMeshNormals, true);
        g_exportOptions->addBool(GeometryExportOptions::kMeshVertexCreases, true);
        g_exportOptions->addBool(GeometryExportOptions::kMeshEdgeCreases, true);
        g_exportOptions->addBool(GeometryExportOptions::kMeshUvs, true);
        g_exportOptions->addBool(GeometryExportOptions::kMeshUvOnly, false);
        g_exportOptions->addBool(GeometryExportOptions::kMeshPointsAsPref, false);
        g_exportOptions->addBool(GeometryExportOptions::kMeshColours, true);
        g_exportOptions->addFloat(GeometryExportOptions::kMeshDefaultColourRGB, 0.18, 3);
        g_exportOptions->addFloat(GeometryExportOptions::kMeshDefaultColourA, 1.0, 3);
        g_exportOptions->addBool(GeometryExportOptions::kCustomColourThreshold, true);
        g_exportOptions->addFloat(
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
