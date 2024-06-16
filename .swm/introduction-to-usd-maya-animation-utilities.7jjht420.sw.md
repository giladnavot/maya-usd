---
title: Introduction to USD Maya Animation Utilities
---

USD Maya Animation Utilities in the maya-usd repository refers to a set of functions and data structures that facilitate the translation of animation data between Maya and USD. These utilities are primarily used to check if attributes are animated, add animated attributes to the animation translator, and handle the conversion of animation data between different units. They are essential for the accurate and efficient transfer of animation data when working with USD in Maya.

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="216">

---

## AnimationTranslator Class

The AnimationTranslator class provides a set of functions for translating animation data. For example, the 'addMultiPlugs' function is used to add multiple Maya attributes to the animation translator if they are animated.

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

<SwmSnippet path="/plugin/al/translators/Camera.cpp" line="453">

---

## Using AnimationTranslator Functions

The functions of the AnimationTranslator class are used in various parts of the codebase. For instance, 'addMultiPlugs' is used within the 'writePrim' function in the Camera.cpp file to add multiple Maya attributes to the animation translator.

```c++
        // Usd attribute
        animTranslator->addMultiPlugs(
            { MPlug(cameraObject, m_nearDistance), MPlug(cameraObject, m_farDistance) },
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="101">

---

## Animation Check Functions

The AnimationTranslator class also provides functions to check if a certain attribute is animated. The 'isAnimated' function, for example, returns true if the attribute is animated. This is used to determine whether to add the attribute to the animation translator.

```c
    /// \brief  returns true if the attribute is animated
    /// \param  node the node which contains the attribute to test
    /// \param  attr the attribute handle
    /// \param  assumeExpressionIsAnimated if we encounter an expression, assume that the attribute
    /// is animated (true) or
    ///         static (false).
    /// \return true if the attribute was found to be animated
    /// This test only covers the situation that your attribute is actually animated by some types
    /// of nodes, e.g. animCurves or expression, or source attribute's full-name match a certain
    /// string. But in reality the control network might be really complicated and heavily
    /// customized thus it might go far beyond the situation we can cover here.
    static bool isAnimated(
        const MObject& node,
        const MObject& attr,
        const bool     assumeExpressionIsAnimated = true)
    {
        return isAnimated(MPlug(node, attr), assumeExpressionIsAnimated);
    }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="298">

---

## Animation Data Structures

The AnimationTranslator class also defines several data structures for storing animated attributes. These include 'm_animatedPlugs', 'm_scaledAnimatedPlugs', 'm_animatedTransformPlugs', and 'm_animatedMeshes'. These data structures are used to store the Maya attributes that are found to be animated.

```c
    PlugAttrVector       m_animatedPlugs;
    PlugAttrScaledVector m_scaledAnimatedPlugs;
    PlugAttrVector       m_animatedTransformPlugs;
    MeshAttrVector       m_animatedMeshes;
    WorldSpaceAttrVector m_worldSpaceOutputs;
```

---

</SwmSnippet>

# AnimationTranslator Functions

This section will cover the main functions of the AnimationTranslator class in the USD Maya Animation Utilities.

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="101">

---

## isAnimated

The `isAnimated` function checks if a Maya attribute is animated. It takes as parameters the node which contains the attribute to test, the attribute handle, and a boolean to indicate if an expression should be assumed to be animated. The function returns true if the attribute is found to be animated.

```c
    /// \brief  returns true if the attribute is animated
    /// \param  node the node which contains the attribute to test
    /// \param  attr the attribute handle
    /// \param  assumeExpressionIsAnimated if we encounter an expression, assume that the attribute
    /// is animated (true) or
    ///         static (false).
    /// \return true if the attribute was found to be animated
    /// This test only covers the situation that your attribute is actually animated by some types
    /// of nodes, e.g. animCurves or expression, or source attribute's full-name match a certain
    /// string. But in reality the control network might be really complicated and heavily
    /// customized thus it might go far beyond the situation we can cover here.
    static bool isAnimated(
        const MObject& node,
        const MObject& attr,
        const bool     assumeExpressionIsAnimated = true)
    {
        return isAnimated(MPlug(node, attr), assumeExpressionIsAnimated);
    }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="152">

---

## addPlug

The `addPlug` function adds a Maya attribute plug to the AnimationTranslator if the plug is animated. It takes as parameters the plug to test, the corresponding USD attribute to write the animation data into if the plug is animated, and a boolean to indicate if an expression should be assumed to be animated.

```c
    /// \brief  add a plug to the animation translator (if the plug is animated)
    /// \param  plug the maya attribute to test
    /// \param  attribute the corresponding maya attribute to write the anim data into if the plug
    /// is animated \param  assumeExpressionIsAnimated if we encounter an expression, assume that
    /// the attribute is animated (true) or
    ///         static (false).
    inline void
    addPlug(const MPlug& plug, const UsdAttribute& attribute, const bool assumeExpressionIsAnimated)
    {
        if (m_animatedPlugs.find(plug) != m_animatedPlugs.end())
            return;
        if (isAnimated(plug, assumeExpressionIsAnimated))
            m_animatedPlugs.emplace(plug, attribute);
    }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="267">

---

## addMesh

The `addMesh` function adds a Maya mesh to the AnimationTranslator. It takes as parameters the path to the animated Maya mesh and the corresponding USD attribute to write the animation data into.

```c
    /// \brief  add a mesh to the animation translator
    /// \param  path the path to the animated maya mesh
    /// \param  attribute the corresponding maya attribute to write the anim data into if the plug
    /// is animated
    inline void addMesh(const MDagPath& path, const UsdAttribute& attribute)
    {
        if (m_animatedMeshes.find(path) == m_animatedMeshes.end())
            m_animatedMeshes.emplace(path, attribute);
    }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="277">

---

## addWorldSpace

The `addWorldSpace` function adds a Maya DAG path to the AnimationTranslator to be exported as a set of world space matrix keyframes. It takes as parameters the path to the animated Maya node and the corresponding USD attribute to write the animation data into.

```c
    /// \brief  add a dag path to be exported as a set of world space matrix keyframes.
    /// \param  path the path to the animated maya mesh
    /// \param  attribute the corresponding maya attribute to write the anim data into if the plug
    /// is animated
    inline void addWorldSpace(const MDagPath& path, const UsdAttribute& attribute)
    {
        if (m_worldSpaceOutputs.find(path) == m_worldSpaceOutputs.end())
            m_worldSpaceOutputs.emplace(path, attribute);
    }
```

---

</SwmSnippet>

# USD Maya Animation Utilities

Understanding USD Maya Animation Utilities

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/NurbsCurveUtils.cpp" line="58">

---

## copyPoints

The `copyPoints` function is used to copy the control vertices from a Maya NURBS curve to a USD attribute. It first retrieves the control vertices from the Maya curve, then converts the Maya CVs from a double vector to a float vector, and finally sets the converted data to the USD attribute.

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

The `copyExtent` function is used to copy the extent of a Maya NURBS curve to a USD NurbsCurves primitive. It first retrieves the control vertices from the Maya curve, then converts the Maya CVs from a double vector to a float vector. It then calculates the extent of the curve and sets it to the USD NurbsCurves primitive.

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
