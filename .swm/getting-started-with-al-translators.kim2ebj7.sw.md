---
title: Getting Started with AL Translators
---
The 'al Translators' in the maya-usd repository refers to a set of translation utilities that are part of the Animal Logic's (AL) plugin. These translators are used to convert data between Maya's internal format and Pixar's Universal Scene Description (USD) format. They are organized in the 'AL::usdmaya::fileio::translators' namespace and include various options for geometry export, subdivision schemes, and other common translation tasks. The translators are also used in handling Maya references, which are a type of node in Maya that reference an external file.

<SwmSnippet path="/plugin/al/translators/CommonTranslatorOptions.h" line="9">

---

## Geometry Export Options

The 'GeometryExportOptions' namespace within 'al Translators' provides a set of options for exporting geometry data from Maya to USD. These options include exporting mesh geometry, mesh face connects, mesh points, mesh extents, mesh normals, and more.

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

## Geometry Import Options

The 'GeometryImportOptions' namespace within 'al Translators' provides a set of options for importing geometry data from USD to Maya. These options include importing meshes and curves.

```c
namespace GeometryImportOptions {
static constexpr const char* const kMeshes = "Import Meshes"; ///< the import meshes option name
static constexpr const char* const kNurbsCurves
    = "Import Curves"; ///< the import curves option name
} // namespace GeometryImportOptions
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/translators/CommonTranslatorOptions.cpp" line="22">

---

## Subdivision Schemes

The 'g_subdivisionSchemes' variable within 'al Translators' defines the subdivision schemes used in the translation of geometry data. These schemes include 'default', 'catmullClark', and 'none'.

```c++
//----------------------------------------------------------------------------------------------------------------------
static const char* const g_subdivisionSchemes[] = { "default", // Do not author opinion
                                                    UsdGeomTokens->catmullClark.GetText(),
                                                    UsdGeomTokens->none.GetText(),
```

---

</SwmSnippet>

# AL Translators

Understanding AL Translators

<SwmSnippet path="/plugin/al/translators/plugInfo.json.in" line="6">

---

## AL::usdmaya::fileio::translators::MayaReference

The `AL::usdmaya::fileio::translators::MayaReference` is a translator that provides import translation for AL_usd_MayaReference. It is based on the TranslatorBase.

```in
                    "AL::usdmaya::fileio::translators::MayaReference": {
                        "bases": ["AL::usdmaya::fileio::translators::TranslatorBase"]
                    },
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/translators/plugInfo.json.in" line="9">

---

## AL::usdmaya::fileio::translators::Camera

The `AL::usdmaya::fileio::translators::Camera` is a translator that provides import translation for Camera objects in Maya. It is also based on the TranslatorBase.

```in
                    "AL::usdmaya::fileio::translators::Camera": {
                        "bases": ["AL::usdmaya::fileio::translators::TranslatorBase"]
                    },
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
