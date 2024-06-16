---
title: Role of TF_REGISTRY_FUNCTION in Dependency Management
---

This document will cover the topic of `TF_REGISTRY_FUNCTION` in the maya-usd repository. We'll cover:

1. What is `TF_REGISTRY_FUNCTION`
2. How `TF_REGISTRY_FUNCTION` is used in the codebase
3. Examples of `TF_REGISTRY_FUNCTION` usage.

# What is `TF_REGISTRY_FUNCTION`

`TF_REGISTRY_FUNCTION` is a macro used in the Pixar USD (Universal Scene Description) framework. It is used to define and register types within the Tf (transfer function) system. This is crucial for the type introspection capabilities of the USD framework.

# How `TF_REGISTRY_FUNCTION` is used in the codebase

`TF_REGISTRY_FUNCTION` is used throughout the maya-usd codebase to define and register various types. It is typically used in conjunction with `TfType::Define` to define a new type and its base type. This is crucial for the type introspection capabilities of the USD framework.

<SwmSnippet path="/lib/usd/hdMaya/adapters/meshAdapter.cpp" line="445">

---

# Examples of `TF_REGISTRY_FUNCTION` usage

Here, `TF_REGISTRY_FUNCTION` is used to define `HdMayaMeshAdapter` as a new type with `HdMayaShapeAdapter` as its base type.

```c++
TF_REGISTRY_FUNCTION(TfType)
{
    TfType::Define<HdMayaMeshAdapter, TfType::Bases<HdMayaShapeAdapter>>();
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/usd/hdMaya/adapters/shapeAdapter.cpp" line="26">

---

In this example, `TF_REGISTRY_FUNCTION` is used to define `HdMayaShapeAdapter` as a new type with `HdMayaDagAdapter` as its base type.

```c++
PXR_NAMESPACE_OPEN_SCOPE

TF_REGISTRY_FUNCTION(TfType)
{
    TfType::Define<HdMayaShapeAdapter, TfType::Bases<HdMayaDagAdapter>>();
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/FrameRange.cpp" line="25">

---

Here, `TF_REGISTRY_FUNCTION` is used to define `AL_usd_FrameRange` as a new type with `UsdTyped` as its base type. It also adds an alias for the type under `UsdSchemaBase`.

```c++
// Register the schema with the TfType system.
TF_REGISTRY_FUNCTION(TfType)
{
    TfType::Define<AL_usd_FrameRange, TfType::Bases<UsdTyped>>();

    // Register the usd prim typename as an alias under UsdSchemaBase. This
    // enables one to call
    // TfType::Find<UsdSchemaBase>().FindDerivedByName("ALFrameRange")
    // to find TfType<AL_usd_FrameRange>, which is how IsA queries are
    // answered.
    TfType::AddAlias<UsdSchemaBase, AL_usd_FrameRange>("ALFrameRange");
}
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
