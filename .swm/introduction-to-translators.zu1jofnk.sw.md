---
title: Introduction to Translators
---

Translators in the maya-usd repository refer to a set of functionalities that facilitate the conversion of data between Maya and USD formats. They are encapsulated within the 'translators' namespace under 'AL::usdmaya::fileio'. The translators handle different types of data, including geometry and animation data. For instance, the 'GeometryExportOptions' and 'GeometryImportOptions' namespaces define options for exporting and importing geometry data respectively. Variables like 'g_exportOptions' and 'g_importOptions' are used to manage these options during the translation process.

<SwmSnippet path="/plugin/al/translators/CommonTranslatorOptions.h" line="9">

---

# GeometryExportOptions

The GeometryExportOptions namespace defines the options for exporting geometry data from Maya to USD. Each option is represented as a constant string, such as 'Meshes' for exporting mesh geometry, 'Mesh Points' for exporting mesh points, and so on.

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

# GeometryImportOptions

Similarly, the GeometryImportOptions namespace defines the options for importing geometry data from USD to Maya. It includes options like 'Import Meshes' for importing mesh geometry and 'Import Curves' for importing curves.

```c
namespace GeometryImportOptions {
static constexpr const char* const kMeshes = "Import Meshes"; ///< the import meshes option name
static constexpr const char* const kNurbsCurves
    = "Import Curves"; ///< the import curves option name
} // namespace GeometryImportOptions
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/translators/CommonTranslatorOptions.cpp" line="17">

---

# Using the Translators

The translators can be used by accessing these options. For example, the `g_importOptions` variable can be used to specify the options for importing geometry data.

```c++
AL::maya::utils::PluginTranslatorOptions* g_importOptions = 0;
```

---

</SwmSnippet>

# Translators Functions

The 'Translators' in the 'maya-usd' repository are responsible for translating data between Maya and USD formats. The main functions involved in this process are 'registerCommonTranslatorOptions' and 'addBool'.

<SwmSnippet path="/plugin/al/translators/CommonTranslatorOptions.cpp" line="31">

---

## registerCommonTranslatorOptions

The 'registerCommonTranslatorOptions' function is responsible for registering the common options for both import and export translators. It sets up the options for geometry export and import, such as whether to export/import meshes, normals, UVs, etc. It uses the 'addBool' function to add these options.

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

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/FileTranslatorOptions.cpp" line="169">

---

## addBool

The 'addBool' function is used by 'registerCommonTranslatorOptions' to add a boolean option to the translator options. It takes a nice name and a default value as parameters, and adds the option to the last frame of the options.

```c++
bool FileTranslatorOptions::addBool(const char* niceName, bool defaultValue)
{
    if (m_frames.empty())
        return false;

    FrameLayout&        frame = lastFrame();
    FrameLayout::Option option;

    option.optionName = niceNameToOptionString(niceName);
    option.niceName = niceName;
    option.type = kBool;
    option.defaultBool = defaultValue;
    option.enumValues = 0;
    if (hasOption(option.optionName)) {
        MString msg("FileTranslatorOptions: cannot register the same bool option twice: ^1");
        msg.format(option.optionName);
        MGlobal::displayError(msg);
        return false;
    }

    frame.m_options.push_back(option);
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
