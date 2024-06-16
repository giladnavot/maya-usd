---
title: Overview of USD Maya Attribute Utilities
---

USD Maya Attribute Utilities, represented in the 'maya-usd' repository by the 'DgNodeHelper' class, is a utility class that provides support for setting and getting attributes. It contains methods to retrieve and set various types of array data from array attributes, such as boolean values, 8 bit char values, 16 bit integer values, 32 bit integer values, and 64 bit integer values. This class is used throughout the codebase to interact with Maya's node attributes, making it a crucial part of the USD Maya plugin's functionality.

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/DgNodeHelper.h" line="43">

---

# DgNodeHelper Class

The `DgNodeHelper` class provides a set of utility methods to get and set attribute values. It includes methods to retrieve and set arrays of different data types, such as boolean, integer, and float.

```c
struct DgNodeHelper
{
public:
    /// ctor
    DgNodeHelper() { }

    /// dtor
    virtual ~DgNodeHelper() { }

    //--------------------------------------------------------------------------------------------------------------------
    /// \name   Methods to get array data from array attributes
    //-----------------------------------------`---------------------------------------------------------------------------

    /// \brief  retrieve an array of boolean values from an attribute in Maya
    /// \param  node the maya node on which the attribute you are interested in exists
    /// \param  attr the handle to the array attribute. This will either be an MObject for a custom
    /// maya attribute,
    ///         a handle queried via the MNodeClass interface, or a dynamically added attribute
    /// \param  values the returned array of values
    /// \return MS::kSuccess if ok
    AL_USDMAYA_UTILS_PUBLIC
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/DgNodeHelper.cpp" line="47">

---

# Using DgNodeHelper

Here is an example of how to use the `DgNodeHelper` class. The `setFloat` method is used to set a float value to a Maya attribute.

```c++
MStatus DgNodeHelper::setFloat(const MObject node, const MObject attr, float value)
{
    const char* const errorString = "float error";
    MPlug             plug(node, attr);
    AL_MAYA_CHECK_ERROR(plug.setValue(value), errorString);
    return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setAngle(const MObject node, const MObject attr, MAngle value)
{
    const char* const errorString = "DgNodeHelper::setAngle";
    MPlug             plug(node, attr);
    AL_MAYA_CHECK_ERROR(plug.setValue(value), errorString);
    return MS::kSuccess;
}

//----------------------------------------------------------------------------------------------------------------------
MStatus DgNodeHelper::setTime(const MObject node, const MObject attr, MTime value)
{
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="216">

---

# Adding Multiple Plugs

The `addMultiPlugs` method is used to add multiple plugs to the animation translator if they are animated. It first checks if the attribute is already in the `m_animatedMultiPlugs` map. If not, it checks if any of the plugs are animated. If they are, it adds them to the `m_animatedMultiPlugs` map.

```c
    /// \brief  add plugs to the animation translator (if plugs are animated)
    ///         values of plugs will be mapped to a single Usd attribute value
    /// \param  plugs the maya attributes to test
    /// \param  attribute the corresponding maya attribute to write the anim data into if plugs are
    /// animated \param  assumeExpressionIsAnimated if we encounter an expression, assume that the
    /// attribute is animated (true) or
    ///         static (false).
    inline void addMultiPlugs(
        const std::vector<MPlug>& plugs,
        const UsdAttribute&       attribute,
        const bool                assumeExpressionIsAnimated)
    {
        if (m_animatedMultiPlugs.find(attribute) != m_animatedMultiPlugs.end()) {
            return;
        }
        bool hasAnimation = false;
        for (const auto& plug : plugs) {
            if (isAnimated(plug, assumeExpressionIsAnimated)) {
                hasAnimation = true;
                break;
            }
```

---

</SwmSnippet>

# USD Maya Attribute Utilities Functions

This section covers the main functions and classes in the USD Maya Attribute Utilities.

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AttributeType.h" line="33">

---

## UsdDataType

The `UsdDataType` is an enumeration class that represents a generalized set of USD attribute types. It allows for switch statements, providing a more efficient alternative to the if/else approach when using SdfValueTypeNames. The enum class includes various data types such as boolean, integer, float, string, matrix, vector, token, asset, and color.

```c
enum class UsdDataType : uint32_t
{
    kBool,
    kUChar,
    kInt,
    kUInt,
    kInt64,
    kUInt64,
    kHalf,
    kFloat,
    kDouble,
    kString,
    kMatrix2d,
    kMatrix3d,
    kMatrix4d,
    kQuatd,
    kQuatf,
    kQuath,
    kVec2d,
    kVec2f,
    kVec2h,
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/MeshUtils.h" line="124">

---

## MeshImportContext

The `MeshImportContext` struct is used to import mesh data from USD into Maya. It contains methods to gather face connects and vertices, apply hole faces, vertex normals, and edge creases. It also has a constructor that initializes the import context for the specified mesh.

```c
struct MeshImportContext
{
private:
    MFnMesh            fnMesh;     ///< the maya function set to use when setting the data
    MFloatPointArray   points;     ///< the array of vertices for the mesh being imported
    MVectorArray       normals;    ///< the array of normal vectors for the mesh being imported
    MIntArray          counts;     ///< the number of vertices in each face within the mesh
    MIntArray          connects;   ///< the vertex indices for each face-vertex in the mesh
    const UsdGeomMesh& mesh;       ///< the USD geometry being imported
    MObject            polyShape;  ///< the handle to the created mesh shape
    UsdTimeCode        m_timeCode; ///< the time at which to import the mesh
    AL_USDMAYA_UTILS_PUBLIC
    void gatherFaceConnectsAndVertices();

public:
    /// \brief  constructs the import context for the specified mesh
    /// \param  mesh the usd geometry to import
    /// \param  parentOrOwner the maya transform that will be the parent transform of the geometry
    /// being imported,
    ///         or a mesh data objected created via MFnMeshData.
    /// \param  dagName the name for the new mesh node
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/MeshUtils.h" line="204">

---

## MeshExportContext

The `MeshExportContext` struct is used to export mesh data from Maya into a USD prim. It provides methods to copy crease edges, vertex data, extent data, and normal data from Maya to USD.

```c
struct MeshExportContext
{
public:
    enum CompactionLevel
    {
        kNone,
        kBasic,
        kMedium,
        kFull
    };

    enum SubdivisionScheme
    {
        kSubdDefault,
        kSubdCatmull,
        kSubdNone,
        kSubdLoop,
        kSubdBilinear,
    };

    /// \brief  constructor
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/DgNodeHelper.h" line="43">

---

## DgNodeHelper

The `DgNodeHelper` struct is a utility class that provides support for setting and getting attributes. It includes methods to retrieve an array of boolean values, 8 bit char values, and other types from an attribute in Maya.

```c
struct DgNodeHelper
{
public:
    /// ctor
    DgNodeHelper() { }

    /// dtor
    virtual ~DgNodeHelper() { }

    //--------------------------------------------------------------------------------------------------------------------
    /// \name   Methods to get array data from array attributes
    //-----------------------------------------`---------------------------------------------------------------------------

    /// \brief  retrieve an array of boolean values from an attribute in Maya
    /// \param  node the maya node on which the attribute you are interested in exists
    /// \param  attr the handle to the array attribute. This will either be an MObject for a custom
    /// maya attribute,
    ///         a handle queried via the MNodeClass interface, or a dynamically added attribute
    /// \param  values the returned array of values
    /// \return MS::kSuccess if ok
    AL_USDMAYA_UTILS_PUBLIC
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/Utils.cpp" line="90">

---

## mapUsdPrimToMayaNode

The `mapUsdPrimToMayaNode` function returns the dagPath result of mapping UsdPrim to Maya Object. It is used when the path of the Maya object could not be determined, then the corresponding Maya path is determined using the ProxyShape and the UsdPrim path.

```c++
//----------------------------------------------------------------------------------------------------------------------
MString mapUsdPrimToMayaNode(
    const UsdPrim&        usdPrim,
    const MObject&        mayaObject,
    const MDagPath* const proxyShapeNode)
{
    if (!usdPrim.IsValid()) {
        MGlobal::displayError("mapUsdPrimToMayaNode: Invalid prim!");
        return MString();
    }
    TfToken mayaPathAttributeName("MayaPath");

    UsdStageWeakPtr stage = usdPrim.GetStage();

    MFnDagNode mayaNode(mayaObject);
    MDagPath   mayaDagPath;
    mayaNode.getPath(mayaDagPath);
    std::string mayaElementPath = AL::maya::utils::convert(mayaDagPath.fullPathName());

    if (mayaDagPath.length() == 0 && proxyShapeNode) {
        // Prepend the mayaPathPrefix
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="216">

---

## addMultiPlugs

The `addMultiPlugs` function adds plugs to the animation translator if the plugs are animated. The values of the plugs will be mapped to a single Usd attribute value.

```c
    /// \brief  add plugs to the animation translator (if plugs are animated)
    ///         values of plugs will be mapped to a single Usd attribute value
    /// \param  plugs the maya attributes to test
    /// \param  attribute the corresponding maya attribute to write the anim data into if plugs are
    /// animated \param  assumeExpressionIsAnimated if we encounter an expression, assume that the
    /// attribute is animated (true) or
    ///         static (false).
    inline void addMultiPlugs(
        const std::vector<MPlug>& plugs,
        const UsdAttribute&       attribute,
        const bool                assumeExpressionIsAnimated)
    {
        if (m_animatedMultiPlugs.find(attribute) != m_animatedMultiPlugs.end()) {
            return;
        }
        bool hasAnimation = false;
        for (const auto& plug : plugs) {
            if (isAnimated(plug, assumeExpressionIsAnimated)) {
                hasAnimation = true;
                break;
            }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AttributeType.cpp" line="127">

---

## getAttributeType

The `getAttributeType` function returns the type of a UsdAttribute. It checks if the attribute is valid and then retrieves its type name. If the type name is not found in the usdTypeHashToEnum map, it returns 'kUnknown'.

```c++
//----------------------------------------------------------------------------------------------------------------------
UsdDataType getAttributeType(const UsdAttribute& usdAttr)
{
    if (!usdAttr.IsValid()) {
        return UsdDataType::kUnknown;
    }
    const SdfValueTypeName typeName = usdAttr.GetTypeName();
    const auto             it = usdTypeHashToEnum.find(typeName.GetHash());
    if (it == usdTypeHashToEnum.end()) {
        return UsdDataType::kUnknown;
    }
    return it->second;
}
```

---

</SwmSnippet>

# USD Maya Attribute Utilities

Understanding USD Maya Attribute Utilities

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/NurbsCurveUtils.cpp" line="58">

---

## copyPoints

The `copyPoints` function is used to copy the control vertices of a Maya NURBS curve to a USD attribute. It first retrieves the control vertices from the Maya curve, then converts the Maya CVs to USD points, and finally sets these points to the USD attribute.

```c++
void copyPoints(const MFnNurbsCurve& fnCurve, const UsdAttribute& pointsAttr, UsdTimeCode time)
{
    MPointArray controlVertices;
    fnCurve.getCVs(controlVertices);
    const unsigned int cvCount = controlVertices.length();
    VtArray<GfVec3f>   dataPoints(cvCount);

    float* const        usdPoints = (float* const)dataPoints.cdata();
    const double* const mayaCVs = (const double* const) & controlVertices[0];

    convertDoubleVec4ArrayToFloatVec3Array(mayaCVs, usdPoints, cvCount);
    pointsAttr.Set(dataPoints, time);
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/NurbsCurveUtils.cpp" line="73">

---

## copyExtent

The `copyExtent` function is used to copy the extent of a Maya NURBS curve to a USD attribute. It first retrieves the control vertices from the Maya curve, then converts the Maya CVs to USD points. It then calculates the extent of the curve and sets this extent to the USD attribute.

```c++
void copyExtent(const MFnNurbsCurve& fnCurve, const UsdGeomNurbsCurves& nurbs, UsdTimeCode time)
{
    MPointArray controlVertices;
    fnCurve.getCVs(controlVertices);
    const unsigned int cvCount = controlVertices.length();
    VtArray<GfVec3f>   dataPoints(cvCount);

    float* const        usdPoints = (float* const)dataPoints.cdata();
    const double* const mayaCVs = (const double* const) & controlVertices[0];

    convertDoubleVec4ArrayToFloatVec3Array(mayaCVs, usdPoints, cvCount);

    // Extents calculation requires widths - set default width if empty or unfound for prim
    VtFloatArray curveWidths;
    nurbs.GetWidthsAttr().Get<VtFloatArray>(&curveWidths);
    if (curveWidths.empty()) {
        curveWidths.push_back(1.0f);
    }

    VtArray<GfVec3f> mayaExtent(2);
    UsdGeomCurves::ComputeExtent(dataPoints, curveWidths, &mayaExtent);
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
