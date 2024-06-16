---
title: Exploring the Maya-USD Relationship in Asset Import/Export Functionality
---

This document will cover the following topics:

1. The role of the Maya to USD relationship in import/export functionality
2. How the Maya to USD relationship is implemented in the codebase
3. Examples of the Maya to USD relationship in action.

# The role of the Maya to USD relationship in import/export functionality

The Maya to USD relationship plays a crucial role in the import/export functionality of the maya-usd plugin. This relationship is responsible for translating data between Maya's native format and the Universal Scene Description (USD) format used in Pixar's animation software. This allows for seamless data exchange between these two different systems, enabling artists to work with the same data in both Maya and Pixar's software.

<SwmSnippet path="/lib/mayaUsd/fileio/utils/adaptor.h" line="289">

---

# Implementation of the Maya to USD relationship

The `UsdMayaAdaptor` class in `adaptor.h` is a key component in implementing the Maya to USD relationship. This class provides a USD Prim-like interface for a Maya object, allowing for the reading and writing of Maya attributes as VtValues. This enables the automatic adaptation of USD data into Maya attributes and vice versa.

```c
/// \class UsdMayaAdaptor
/// The UsdMayaAdaptor transparently adapts the interface for a Maya object
/// to a UsdPrim-like interface, allowing you to get and set Maya attributes as
/// VtValues. Via this mechanism, the USD importer can automatically adapt USD
/// data into Maya attributes, and the USD exporter can adapt Maya data back
/// into USD attributes. This is particularly useful for USD- or site-specific
/// data that is not natively handled by Maya. For example, you can use the
/// adaptor to set UsdGeomModelAPI's model draw mode attributes from within
/// Maya, and the exported USD prims will conform to the API schema.
///
/// UsdMayaAdaptor determines the conversion between Maya and USD types by
/// consulting registered metadata fields and schemas. In order to use it with
/// any custom metadata or schemas, you must ensure that they are registered
/// via a plugInfo.json file and loaded by the USD system. If you need to
/// store and retrieve custom/blind data _without_ registering it beforehand,
/// you can use User-Exported Attributes instead.
///
/// The advantage of registering your metadata and schemas is that you can
/// configure the USD importer and exporter to handle known metadata and
/// schemas, enabling round-tripping of site-specific data between USD and Maya
/// without per-file configuration. See the _metadata_ and _apiSchema_ flags on
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes/ProxyUsdGeomCamera.cpp" line="106">

---

# Example of the Maya to USD relationship in action

The `getInternalValue` method in `ProxyUsdGeomCamera.cpp` is an example of the Maya to USD relationship in action. This method retrieves the internal value of a Maya plug and translates it into a USD attribute. This is an example of reading data from Maya and translating it into the USD format.

```c++
// USD -> Maya
bool ProxyUsdGeomCamera::getInternalValue(const MPlug& plug, MDataHandle& dataHandle)
{
    const float mm_to_inches = 0.0393701f;

    MStatus status;
    bool    handledAttribute = false;

    UsdTimeCode usdTime(getTime());

    const UsdGeomCamera camera(getCamera());
    if (!camera)
        return false;

    if (plug == m_nearClipPlane) {
        const UsdAttribute attr(camera.CreateClippingRangeAttr());
        if (attr.GetVariability() != SdfVariability::SdfVariabilityVarying)
            usdTime = UsdTimeCode::Default();
        GfVec2f value;
        if (attr.Get(&value, usdTime)) {
            dataHandle.setMDistance(MDistance(value[0], MDistance::kCentimeters));
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
