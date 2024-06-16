---
title: Introduction to Translators
---

Translators in the maya-usd repository refer to a set of helper functions that facilitate the conversion of data between Maya and USD formats. They are used to read and write different types of data, such as geometry, materials, lights, and animations. For example, the `UsdMayaTranslatorGprim` struct provides helper functions for reading `UsdGeomGprim` data into Maya. Similarly, the `UsdMayaTranslatorMaterial` struct provides helper functions for reading `UsdShadeMaterial` data. Translators play a crucial role in ensuring that data is accurately and efficiently converted between the two formats, preserving the integrity of the data and enabling seamless workflows in the Maya environment.

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorCurves.h" line="30">

---

# TranslatorCurves

The `UsdMayaTranslatorCurves` struct provides helper functions for creating UsdCurves. It has a static `Create` function that takes in UsdGeomCurves, a parent MObject, and reader arguments and context. This function is used to create a NurbsCurve under the parent node.

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

# TranslatorMeshRead and TranslatorMeshWrite

The `TranslatorMeshRead` class provides helper functions for translating UsdGeomMesh prims into Maya meshes. The `TranslatorMeshWrite` class, on the other hand, is used for writing Maya mesh data to USD.

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

The `UsdMayaTranslatorLight` struct provides helper functions for translating to/from UsdLux. It has several static functions for writing light attributes and importing a UsdLuxLightAPI schema as a corresponding Maya light.

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

The `UsdMayaTranslatorMaterial` struct provides helper functions for reading UsdShadeMaterial. It has static functions for reading and assigning materials, and for exporting shading engines.

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

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorMayaReference.h" line="33">

---

# TranslatorMayaReference

The `UsdMayaTranslatorMayaReference` struct provides helper functions for reading UsdGeomGprim. It has static functions for creating Maya references, unloading Maya references, and updating Maya references.

```c
#define PXRUSDMAYA_TRANSLATOR_GPRIM_H

#include <mayaUsd/base/api.h>
#include <mayaUsd/fileio/primReaderContext.h>
#include <mayaUsd/fileio/primWriterContext.h>

#include <pxr/pxr.h>
#include <pxr/usd/usd/prim.h>

#include <maya/MFnReference.h>
#include <maya/MObject.h>

PXR_NAMESPACE_OPEN_SCOPE

/// \brief Provides helper functions for reading UsdGeomGprim.
struct UsdMayaTranslatorMayaReference
{
    MAYAUSD_CORE_PUBLIC
    static MStatus CreateMayaReference(
        const UsdPrim& prim,
        MObject&       parent,
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.h" line="52">

---

# TranslatorUtil

The `UsdMayaTranslatorUtil` struct provides helper functions for other readers/writers to use. It has several static functions for creating nodes, shader nodes, and for reading and writing shader attributes.

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

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorXformable.h" line="33">

---

# TranslatorXformable

The `UsdMayaTranslatorXformable` struct provides helper functions for reading UsdGeomXformable. It has a static `Read` function that reads xform attributes from a UsdGeomXformable and converts them into Maya transform values.

```c
struct UsdMayaTranslatorXformable
{
    /// \brief reads xform attributes from \p xformable and converts them into
    /// maya transform values.
    MAYAUSD_CORE_PUBLIC
    static void Read(
        const UsdGeomXformable&      xformable,
        MObject                      mayaNode,
        const UsdMayaPrimReaderArgs& args,
        UsdMayaPrimReaderContext*    context);

    /// \brief Convenince function for decomposing \p usdMatrix.
    MAYAUSD_CORE_PUBLIC
    static bool ConvertUsdMatrixToComponents(
        const GfMatrix4d& usdMatrix,
        GfVec3d*          trans,
        GfVec3d*          rot,
        GfVec3d*          scale);
};

PXR_NAMESPACE_CLOSE_SCOPE
```

---

</SwmSnippet>

# Functions Overview

This section discusses the main functions used in the 'maya-usd' repository.

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="58">

---

## CreateTransformNode

The `CreateTransformNode` function is used to create a transform node for a given USD primitive. It checks if the USD primitive is a valid `UsdGeomXformable` and then calls the `CreateNode` function to create the transform node. It also reads xformable attributes from the USD primitive onto the transform node.

```c++
/* static */
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

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="162">

---

## CreateNode

The `CreateNode` function is used to create a node for a given USD primitive. It takes the USD primitive, node type name, parent node, context, status, and maya node object as parameters. It then calls another overloaded version of `CreateNode` function to create the node.

```c++
/* static */
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

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="382">

---

## \_ReadFromUsdAttribute

The `_ReadFromUsdAttribute` function reads from the USD attribute and writes into the Maya plug. It uses a converter to convert the USD attribute to the Maya plug. If the conversion is not successful, it returns false and populates the reason for failure.

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

The `_ReadProperty` function reads the property identified by shader property from the USD primitive onto the Maya dependency node. It uses the converter to read from the USD attribute and set it in Maya.

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

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorUtil.cpp" line="501">

---

## \_WriteProperty

The `_WriteProperty` function writes the property specified via the shader property from the Maya dependency node onto the USD primitive. It gets the value from the Maya plug using the `_GetValue` function and if the value is not empty, it creates the USD attribute.

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

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
