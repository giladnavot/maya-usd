---
title: Writing RfM Light Attributes to a USD Prim
---
This document will cover the process of writing RenderMan for Maya (RfM) light attributes to a USD Prim, which includes:

1. Writing shader attributes to USD Prim
2. Writing AOV light attributes
3. Setting light Prim attributes
4. Creating and getting attributes using Schema API Adaptor
5. Adapting Maya attributes to USD

```mermaid
graph TD;
subgraph lib/mayaUsd/fileio/translators
  Write:::mainFlowStyle --> _WriteShaderAttributesToUsdPrim_deprecated
end
subgraph lib/mayaUsd/fileio/translators
  _WriteShaderAttributesToUsdPrim_deprecated:::mainFlowStyle --> _WriteAovLight
end
subgraph lib/mayaUsd/fileio/translators
  _WriteShaderAttributesToUsdPrim_deprecated:::mainFlowStyle --> _WriteEnvDayLight
end
subgraph lib/mayaUsd/fileio/translators
  _WriteEnvDayLight:::mainFlowStyle --> _SetLightPrimAttr
end
subgraph lib/mayaUsd/fileio
  _SetLightPrimAttr:::mainFlowStyle --> CreateAttribute
end
subgraph lib/mayaUsd/fileio
  CreateAttribute:::mainFlowStyle --> GetAttribute
end
subgraph lib/mayaUsd/fileio
  GetAttribute:::mainFlowStyle --> GetConvertibleAttribute
end
subgraph lib/mayaUsd/fileio
  GetConvertibleAttribute:::mainFlowStyle --> UsdMayaAttributeAdaptor
end
  UsdMayaAttributeAdaptor:::mainFlowStyle --> ...

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorRfMLightDeprecated.cpp" line="1818">

---

# Writing shader attributes to USD Prim

The function `_WriteShaderAttributesToUsdPrim_deprecated` is used to write various light attributes such as intensity, exposure, color, temperature, etc. to a USD Lux Light Schema. It also calls `_WriteAovLight` and `_WriteEnvDayLight` functions to write AOV light and environment daylight attributes respectively.

```c++
void _WriteShaderAttributesToUsdPrim_deprecated(
    const MFnDependencyNode& depFn,
    UsdLuxLightAPI&          lightSchema)
{
    _WriteLightIntensity(depFn, lightSchema);
    _WriteLightExposure(depFn, lightSchema);
    _WriteLightDiffuse(depFn, lightSchema);
    _WriteLightSpecular(depFn, lightSchema);
    _WriteLightNormalizePower(depFn, lightSchema);
    _WriteLightColor(depFn, lightSchema);
    _WriteLightTemperature(depFn, lightSchema);

    // XXX: Light filters not yet implemented.
    // XXX: PxrMeshLight geometry not yet implemented.
    // XXX: PxrDomeLight portals not yet implemented.

    _WriteDistantLightAngle(depFn, lightSchema);

    _WriteLightTextureFile(depFn, lightSchema);

    _WriteAovLight(depFn, lightSchema);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorRfMLightDeprecated.cpp" line="560">

---

# Writing AOV light attributes

The function `_WriteAovLight` writes AOV light attributes such as AOV name, in primary hit, in reflection, in refraction, invert, on volume boundaries, use color, and use throughput to the USD Lux Light Schema. It uses `_SetLightPrimAttr` function to set these attributes to the light Prim.

```c++
// AOV LIGHT
static bool _WriteAovLight(const MFnDependencyNode& depFn, UsdLuxLightAPI& lightSchema)
{
    // Early out
    UsdPrim              lightPrim = lightSchema.GetPrim();
    static const TfType& usdSchemaBase = TfType::FindByName(_tokens->UsdSchemaBase);
    static const TfType& pxrAovLightType
        = usdSchemaBase.FindDerivedByName(_tokens->AovLightMayaTypeName);

    const TfType& lightType = usdSchemaBase.FindDerivedByName(lightPrim.GetTypeName());
    if (!lightType.IsA(pxrAovLightType)) {
        return false;
    }

    MStatus status;

    // AOV Name.
    MPlug aovNamePlug = depFn.findPlug(_tokens->AovNamePlugName.GetText(), &status);
    if (status != MS::kSuccess) {
        return false;
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorRfMLightDeprecated.cpp" line="124">

---

# Setting light Prim attributes

The function `_SetLightPrimAttr` is used to set a given attribute to the light Prim. It creates the attribute with the given name, type, variability, and default value, and sets it to the light Prim.

```c++
// Adapted from UsdSchemaBase::_CreateAttr
static UsdAttribute _SetLightPrimAttr(
    UsdPrim&                lightPrim,
    TfToken const&          attrName,
    SdfValueTypeName const& typeName,
    bool                    custom,
    SdfVariability          variability,
    VtValue const&          defaultValue,
    bool                    writeSparsely)
{

    const TfToken& attrToken = _ShaderAttrName(_PrefixRiLightAttrNamespace(attrName));

    if (writeSparsely && !custom) {
        UsdAttribute attr = lightPrim.GetAttribute(attrToken);
        VtValue      fallback;
        if (defaultValue.IsEmpty()
            || (!attr.HasAuthoredValue() && attr.Get(&fallback) && fallback == defaultValue)) {
            return attr;
        }
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/schemaApiAdaptor.cpp" line="85">

---

# Creating and getting attributes using Schema API Adaptor

The `CreateAttribute` function in `UsdMayaSchemaApiAdaptor` is used to create a new attribute for the schema. If the attribute is translatable, it returns the existing attribute, otherwise, it creates a new dynamic attribute. The `GetAttribute` function is used to get an existing attribute. If the attribute is translatable, it returns the convertible attribute, otherwise, it gets the dynamic attribute.

```c++
UsdMayaAttributeAdaptor
UsdMayaSchemaApiAdaptor::CreateAttribute(const TfToken& attrName, MDGModifier& modifier)
{
    TfToken mayaAttribute = GetMayaNameForUsdAttrName(attrName);
    if (!mayaAttribute.IsEmpty()) {
        // Transatable attribute always exists:
        return GetAttribute(attrName);
    } else {
        // Untranslatable attributes are handled with dynamic attributes.
        MObjectHandle objectHandle(GetMayaObjectForSchema());
        if (!objectHandle.isValid()) {
            // It is possible that the referenced object got removed, making this call impossible.
            TF_CODING_ERROR(
                "Could not find object referenced in schema '%s'", _schemaName.GetText());
            return UsdMayaAttributeAdaptor();
        }
        UsdMayaSchemaAdaptor genericAdaptor { objectHandle, _schemaName, _schemaDef };
        return genericAdaptor.CreateAttribute(attrName, modifier);
    }
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/adaptor.cpp" line="978">

---

# Adapting Maya attributes to USD

The `UsdMayaAttributeAdaptor` class is used to adapt Maya attributes to USD. It holds a Maya plug, a Maya node, a USD attribute, and a USD attribute definition.

```c++
UsdMayaAttributeAdaptor::UsdMayaAttributeAdaptor()
    : _plug()
    , _node()
    , _attr()
    , _attrDef()
{
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="flows"><sup>Powered by [Swimm](/)</sup></SwmMeta>
