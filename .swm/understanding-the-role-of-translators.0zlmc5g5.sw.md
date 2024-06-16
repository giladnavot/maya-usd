---
title: Understanding the Role of Translators
---
Translators in the maya-usd repository refer to a set of helper functions that facilitate the conversion of data between Maya and USD formats. They are used to read and write different types of data, such as geometry, materials, lights, and animations. For example, the `UsdMayaTranslatorMaterial` helps in reading UsdShadeMaterial and writing shading engines to the USD stage. Similarly, `UsdMayaTranslatorMeshRead` and `UsdMayaTranslatorMeshWrite` assist in translating UsdGeomMesh prims into Maya meshes and vice versa. Translators play a crucial role in ensuring data integrity and consistency when transferring data between Maya and USD.

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorCurves.h" line="30">

---

# TranslatorCurves

The `UsdMayaTranslatorCurves` struct provides a helper function for creating UsdCurves. It has a static `Create` function that takes in UsdGeomCurves, a parent MObject, and reader arguments and context. This function is used to create a NurbsCurve under the parent node.

```c
/// \brief Provides helper functions for creating UsdCurves
struct UsdMayaTranslatorCurves
{
    /// \brief Creates a NurbsCurve under \p parentNode.
    MAYAUSD_CORE_PUBLIC
    static bool Create(
        const UsdGeomCurves&         curves,
        MObject                      parentNode,
        const UsdMayaPrimReaderArgs& args,
        UsdMayaPrimReaderContext*    context);
};
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorMesh.h" line="35">

---

# TranslatorMesh

The `TranslatorMeshRead` and `TranslatorMeshWrite` classes provide helper functions for translating UsdGeomMesh prims into Maya meshes and vice versa. They have several public methods for reading and writing mesh data, including `meshObject`, `blendObject`, `pointBasedDeformerNode`, and `usdMesh`.

```c
/// Provides helper functions for translating UsdGeomMesh prims into Maya
/// meshes.
class MAYAUSD_CORE_PUBLIC TranslatorMeshRead
{
public:
    TranslatorMeshRead(
        const UsdGeomMesh&        mesh,
        const UsdPrim&            prim,
        const MObject&            transformObj,
        const MObject&            stageNode,
        const GfInterval&         frameRange,
        bool                      wantCacheAnimation,
        UsdMayaPrimReaderContext* context,
        MStatus*                  status = nullptr);

    ~TranslatorMeshRead() = default;

    MAYAUSD_DISALLOW_COPY_MOVE_AND_ASSIGNMENT(TranslatorMeshRead);

    MObject meshObject() const;

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorLight.h" line="39">

---

# TranslatorLight

The `UsdMayaTranslatorLight` struct provides helper functions for translating to/from UsdLux. It has several static methods for writing light attributes from a generic Maya MFnLight to UsdLuxLightAPI, and for importing a UsdLuxLightAPI schema as a corresponding Maya light.

```c
/// \brief Provides helper functions for translating to/from UsdLux
struct UsdMayaTranslatorLight
{
    /// Exports the UsdLuxLightAPI schema attributes from a generic Maya MFnLight.
    /// This function should be called when exporting to any of the specialized light schemas.
    /// Return true if all the parameters were exported properly.
    MAYAUSD_CORE_PUBLIC
    static bool WriteLightAttrs(
        const UsdTimeCode&         usdTime,
        const UsdLuxLightAPI&      usdLight,
        MFnLight&                  mayaLight,
        FlexibleSparseValueWriter* valueWriter = nullptr);

    /// Exports Maya's directional light attributes using UsdLuxDistantLight schema
    MAYAUSD_CORE_PUBLIC
    static bool WriteDirectionalLightAttrs(
        const UsdTimeCode&         usdTime,
        const UsdLuxDistantLight&  usdLight,
        MFnDirectionalLight&       mayaLight,
        FlexibleSparseValueWriter* valueWriter = nullptr);

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorMaterial.h" line="34">

---

# TranslatorMaterial

The `UsdMayaTranslatorMaterial` struct provides helper functions for reading UsdShadeMaterial. It has static methods for reading material data from a UsdShadeMaterial and assigning it to a Maya node, and for finding shadingEngines in the Maya scene and exporting them to the USD stage.

```c
/// \brief Provides helper functions for reading UsdShadeMaterial
struct UsdMayaTranslatorMaterial
{
    /// Reads \p material according to the shading mode found in \p jobArguments. Some shading modes
    /// may want to know the \p boundPrim. This returns an MObject that is the Maya shadingEngine
    /// that corresponds to \p material.
    MAYAUSD_CORE_PUBLIC
    static MObject Read(
        const UsdMayaJobImportArgs& jobArguments,
        const UsdShadeMaterial&     material,
        const UsdGeomGprim&         boundPrim,
        UsdMayaPrimReaderContext*   context);

    /// Given a \p prim, assigns a material to it according to the shading mode found in
    /// \p jobArguments. This will see which UsdShadeMaterial is bound to \p prim. If the material
    /// has not been read already, it will read it. The created/retrieved shadingEngine will be
    /// assigned to \p shapeObj.
    MAYAUSD_CORE_PUBLIC
    static bool AssignMaterial(
        const UsdMayaJobImportArgs& jobArguments,
        const UsdGeomGprim&         prim,
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.h" line="52">

---

# TranslatorUtil

The `UsdMayaTranslatorUtil` struct provides a collection of helper functions for other readers/writers to use. These include methods for creating transform nodes, shader nodes, and nodes of a specified type, as well as methods for reading and writing shader attributes.

```c
struct UsdMayaTranslatorUtil
{
    /// \brief Often when creating a prim, we want to first create a Transform
    /// node. This is a small helper to do this. If the \p args provided
    /// indicate that animation should be read, any transform animation from
    /// the prim is transferred onto the Maya transform node. If \p context is
    /// non-NULL, the new Maya node will be registered to the path of
    /// \p usdPrim.
    MAYAUSD_CORE_PUBLIC
    static bool CreateTransformNode(
        const UsdPrim&               usdPrim,
        MObject&                     parentNode,
        const UsdMayaPrimReaderArgs& args,
        UsdMayaPrimReaderContext*    context,
        MStatus*                     status,
        MObject*                     mayaNodeObj);

    /// \brief Creates a "dummy" transform node for the given prim, where the
    /// dummy transform has all transform properties locked.
    /// A UsdMayaAdaptor-compatible attribute for the typeName metadata will
    /// be generated. If \p importTypeName is \c true, this attribute will
```

---

</SwmSnippet>

# Translators Overview

This section covers the main functions used in the translation process between Maya and USD.

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="59">

---

## CreateTransformNode

The `CreateTransformNode` function is used to create a new transform node in Maya from a USD primitive. It checks if the USD primitive is a valid `UsdGeomXformable` and then calls `CreateNode` to create the transform node in Maya. After the node is created, it reads the transformable attributes from the USD primitive and applies them to the newly created Maya node.

```c++
bool UsdMayaTranslatorUtil::CreateTransformNode(
    const UsdPrim&               usdPrim,
    MObject&                     parentNode,
    const UsdMayaPrimReaderArgs& args,
    UsdMayaPrimReaderContext*    context,
    MStatus*                     status,
    MObject*                     mayaNodeObj)
{
    if (!usdPrim || !usdPrim.IsA<UsdGeomXformable>()) {
        return false;
    }

    if (!CreateNode(usdPrim, _DEFAULT_TRANSFORM_TYPE, parentNode, context, status, mayaNodeObj)) {
        return false;
    }

    // Read xformable attributes from the UsdPrim on to the transform node.
    UsdGeomXformable xformable(usdPrim);
    UsdMayaTranslatorXformable::Read(xformable, *mayaNodeObj, args, context);

    return true;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="163">

---

## CreateNode

The `CreateNode` function is a helper function that creates a new Maya node of a specified type from a USD primitive. It calls another overloaded version of `CreateNode` that takes the path of the USD primitive instead of the primitive itself.

```c++
bool UsdMayaTranslatorUtil::CreateNode(
    const UsdPrim&            usdPrim,
    const MString&            nodeTypeName,
    MObject&                  parentNode,
    UsdMayaPrimReaderContext* context,
    MStatus*                  status,
    MObject*                  mayaNodeObj)
{
    return CreateNode(usdPrim.GetPath(), nodeTypeName, parentNode, context, status, mayaNodeObj);
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="387">

---

## \_ReadFromUsdAttribute

The `_ReadFromUsdAttribute` function reads data from a USD attribute and writes it into a Maya plug. It uses a converter to handle the conversion between USD and Maya data types. If the conversion is not supported, it returns false and provides a reason for the failure.

```c++
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
                usdAttr.GetPath().GetText());
        }
        return false;
    }
    return true;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="419">

---

## \_ReadProperty

The `_ReadProperty` function reads a property from a USD shader and sets it in Maya. It first retrieves the USD attribute and the corresponding Maya plug. Then it uses `_ReadFromUsdAttribute` to read the data from the USD attribute and set it in the Maya plug.

```c++
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
    MPlug   attrPlug = depFn.findPlug(mayaAttrName.GetText(), &status);
    if (status != MS::kSuccess) {
        if (reason) {
            *reason = TfStringPrintf(
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="501">

---

## \_WriteProperty

The `_WriteProperty` function writes a property from a Maya node to a USD shader. It retrieves the Maya plug and the corresponding USD attribute. Then it uses `_GetValue` to get the data from the Maya plug and sets it in the USD attribute.

```c++
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
        }
    }
    return true;
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
