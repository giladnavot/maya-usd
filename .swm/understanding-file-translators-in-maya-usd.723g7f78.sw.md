---
title: Understanding File Translators in Maya-USD
---

File Translators in the maya-usd repository refer to a set of helper functions that facilitate the conversion of data between Maya and USD formats. These translators handle different types of data such as geometry, materials, and animations. For instance, the `UsdMayaTranslatorGprim` struct provides helper functions for reading `UsdGeomGprim`, which represents geometric primitives in USD. Similarly, `UsdMayaTranslatorMaterial` provides functions for reading and writing material data. The translators ensure that data is correctly interpreted and represented when moving between Maya and USD, preserving the integrity of the 3D scenes.

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorMaterial.cpp" line="172">

---

# Material Translator

This section of the code in `translatorMaterial.cpp` defines functions for reading and assigning materials from USD data. It also includes a function for exporting shading engines from Maya to USD.

```c++
static _UVBindings
_GetUVBindingsFromMaterial(const UsdShadeMaterial& material, UsdMayaPrimReaderContext* context)
{
    _UVBindings retVal;

    if (!material || !context) {
        return retVal;
    }
    const bool isMergeable = _IsMergeableMaterial(material);
    // Find out the nodes requiring mapping. This code has deep knowledge of how the mappings are
    // exported. See the _UVMappingManager class in ..\shading\shadingModeExporterContext.cpp for
    // details.
    for (const UsdShadeInput& input : material.GetInputs()) {
        const UsdAttribute&      usdAttr = input.GetAttr();
        std::vector<std::string> splitName = usdAttr.SplitName();
        if (splitName.size() != 3
            || (splitName[2] != _tokens->varname.GetString()
                && splitName[2] != _tokens->varnameStr.GetString())) {
            continue;
        }
        VtValue val;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorCurves.cpp" line="35">

---

# Curve Translator

This section of the code in `translatorCurves.cpp` defines functions for creating NurbsCurves in Maya from USD data. It includes functions for converting data, creating the curve shape node, and setting up animation for the curve.

```c++
PXR_NAMESPACE_OPEN_SCOPE

/* static */
bool convertToBezier(MFnNurbsCurve& nurbsCurveFn, MObject& mayaNodeTransformObj, MStatus& status)
{
    MFnNurbsCurve curveFn;
    MFnDagNode    dagFn;
    MObject curveObj = dagFn.create("bezierCurve", "bezierShape1", mayaNodeTransformObj, &status);
    if (status != MS::kSuccess) {
        return false;
    }
    curveFn.setObject(curveObj);
    // Create a nurbs to bezier converter
    MFnDependencyNode convFn;
    convFn.create("nurbsCurveToBezier");
    // Connect the converter between the nurbs and the bezier
    MPlug        convIn = convFn.findPlug("inputCurve", false);
    MPlug        convOut = convFn.findPlug("outputCurve", false);
    MPlug        nurbsOut = nurbsCurveFn.findPlug("local", false);
    MPlug        bezIn = dagFn.findPlug("create", false);
    MDGModifier& dgm = MayaUsd::MDGModifierUndoItem::create("Nurbs curve connections");
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorMesh.cpp" line="46">

---

# Mesh Translator

This section of the code in `translatorMesh.cpp` defines functions for translating UsdGeomMesh prims into Maya meshes. It includes functions for creating the mesh shape node, setting up animation for the mesh, and writing USD type information into the Maya object.

```c++

namespace MAYAUSD_NS_DEF {

MAYAUSD_VERIFY_CLASS_NOT_MOVE_OR_COPY(TranslatorMeshRead);

TranslatorMeshRead::TranslatorMeshRead(
    const UsdGeomMesh&        mesh,
    const UsdPrim&            prim,
    const MObject&            transformObj,
    const MObject&            stageNode,
    const GfInterval&         frameRange,
    bool                      wantCacheAnimation,
    UsdMayaPrimReaderContext* context,
    MStatus*                  status)
    : m_wantCacheAnimation(wantCacheAnimation)
    , m_pointsNumTimeSamples(0u)
{
    MStatus stat { MS::kSuccess };

    // ==============================================
    // construct a Maya mesh
```

---

</SwmSnippet>

# Reading and Writing Data

This section focuses on the functions that are responsible for reading and writing data between Maya and USD.

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="382">

---

## \_ReadFromUsdAttribute

The `_ReadFromUsdAttribute` function is used to read data from a UsdAttribute and write it into a Maya plug. It uses a converter to perform the conversion from USD to Maya data types. If the conversion is unsuccessful, it provides a reason for the failure.

```c++
// Reads from the usdAttribute and writes into attrPlug (using sdfValueTypeName
// to help guide the conversion).
//
// Returns true if successful.  This will return false if it failed and populate
// reason with the reason for failure.
static bool _ReadFromUsdAttribute(
    const UsdAttribute&     usdAttr,
    const SdfValueTypeName& sdfValueTypeName,
    MPlug&                  attrPlug,
    std::string*            reason)
{
    static constexpr bool isArrayPlug = false;
    if (const MayaUsd::Converter* converter
        = MayaUsd::Converter::find(sdfValueTypeName, isArrayPlug)) {
        static const MayaUsd::ConverterArgs args;
        converter->convert(usdAttr, attrPlug, args);
    } else {
        if (reason) {
            *reason = TfStringPrintf(
                "Unsupported type %s on attr %s.",
                sdfValueTypeName.GetAsToken().GetText(),
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="415">

---

## \_ReadProperty

The `_ReadProperty` function reads a property from a USD prim and sets it in Maya. It uses the `_ReadFromUsdAttribute` function to perform the conversion. If the property cannot be read, it provides a reason for the failure.

```c++
// Reads the property identified by shaderProp from the prim onto depFn.
//
// Returns true if successful.  This will return false if it failed and populate
// reason with the reason for failure.
static bool _ReadProperty(
    const SdrShaderPropertyConstPtr& shaderProp,
    const UsdPrim&                   usdPrim,
    const MFnDependencyNode&         depFn,
    std::string*                     reason)
{
    const TfToken          mayaAttrName(shaderProp->GetImplementationName());
    const TfToken          usdAttrName(_ShaderAttrName(shaderProp->GetName()));
    const SdfValueTypeName sdfValueTypeName = shaderProp->GetTypeAsSdfType().first;

    UsdAttribute usdAttr = usdPrim.GetAttribute(usdAttrName);
    if (!usdAttr) {
        // If no attribute, no need to author anything.
        return true;
    }

    MStatus status;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="496">

---

## \_WriteProperty

The `_WriteProperty` function writes a property from a Maya dependency node to a USD prim. It uses the `_GetValue` function to perform the conversion. If the property cannot be written, it provides a reason for the failure.

```c++
// Writes the property specified via the shaderProp from the depFn onto the
// usdPrim.
//
// Returns true if successful.  This will return false if it failed and populate
// reason with the reason for failure.
static bool _WriteProperty(
    const SdrShaderPropertyConstPtr& shaderProp,
    const MFnDependencyNode&         depFn,
    UsdPrim                          usdPrim,
    std::string*                     reason)
{
    const TfToken          mayaAttrName(shaderProp->GetImplementationName());
    const TfToken          usdAttrName(_ShaderAttrName(shaderProp->GetName()));
    const SdfValueTypeName sdfValueTypeName = shaderProp->GetTypeAsSdfType().first;

    MStatus     status;
    const MPlug attrPlug = depFn.findPlug(mayaAttrName.GetText(), &status);
    if (status == MS::kSuccess) {
        const VtValue value = _GetValue(attrPlug, sdfValueTypeName);
        if (!value.IsEmpty()) {
            _CreateAttr(usdPrim, usdAttrName, sdfValueTypeName, value);
```

---

</SwmSnippet>

# Translator Endpoints

Understanding Translator Endpoints

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorGprim.h" line="40">

---

## UsdMayaTranslatorGprim::Read

The `UsdMayaTranslatorGprim::Read` method is used to read data from a UsdGeomGprim object and apply it to a Maya node. This method is typically used when importing data from a USD file into a Maya scene.

```c
    MAYAUSD_CORE_PUBLIC
    static void
    Read(const UsdGeomGprim& gprim, MObject mayaNode, UsdMayaPrimReaderContext* context);

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorGprim.h" line="44">

---

## UsdMayaTranslatorGprim::Write

The `UsdMayaTranslatorGprim::Write` method is used to write data from a Maya node to a UsdGeomGprim object. This method is typically used when exporting data from a Maya scene to a USD file.

```c
    MAYAUSD_CORE_PUBLIC
    static void Write(
        const MObject&            mayaNode,
        const UsdGeomGprim&       gprim,
        UsdMayaPrimWriterContext* context,
        GeomSidedness             sidedness);
};
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
