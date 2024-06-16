---
title: Overview of the API in maya-usd
---
API in the maya-usd repository refers to a set of macros defined in the `lib/mayaUsdAPI/api.h` file. These macros, such as `MAYAUSD_API_PUBLIC` and `MAYAUSD_API_LOCAL`, are used to control the visibility of the functions, classes, or variables that they are declared with. They are used to specify whether these entities are available for other code (public) or only within the same module (local).

Additionally, the `lib/mayaUsdAPI/python/__init__.py` file prepares the Python module `_mayaUsdAPI` which is likely to be a part of the API that allows interaction with the maya-usd plugin using Python.

<SwmSnippet path="/lib/mayaUsdAPI/api.h" line="27">

---

# API Macros

Here we see the definition of the `MAYAUSD_API_PUBLIC` macro. This macro is used to mark a class, structure, or function as part of the public API. It uses compiler-specific attributes to control the visibility of the marked item.

```c
  #ifdef MAYAUSD_API_EXPORT
    #ifdef __GNUC__
      #define MAYAUSD_API_PUBLIC __attribute__ ((dllexport))
    #else
      #define MAYAUSD_API_PUBLIC __declspec(dllexport)
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/api.h" line="42">

---

Here we see the definition of the `MAYAUSD_API_LOCAL` macro. This macro is used to mark a class, structure, or function as hidden and not part of the public API. It uses compiler-specific attributes to control the visibility of the marked item.

```c
  #if __GNUC__ >= 4
    #define MAYAUSD_API_PUBLIC __attribute__ ((visibility ("default")))
    #define MAYAUSD_API_LOCAL  __attribute__ ((visibility ("hidden")))
#else
    #define MAYAUSD_API_PUBLIC
    #define MAYAUSD_API_LOCAL
#endif
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/api.h" line="51">

---

Here we see the definition of the `MAYAUSD_API_TEMPLATE_CLASS` and `MAYAUSD_API_TEMPLATE_STRUCT` macros. These macros are used to control the visibility and export of template classes and structures.

```c
#ifdef MAYAUSD_API_EXPORT
  #define MAYAUSD_API_TEMPLATE_CLASS(...) ARCH_EXPORT_TEMPLATE(class, __VA_ARGS__)
  #define MAYAUSD_API_TEMPLATE_STRUCT(...) ARCH_EXPORT_TEMPLATE(struct, __VA_ARGS__)
```

---

</SwmSnippet>

# API Functions Overview

The API functions in the maya-usd repository provide a range of functionalities for interacting with the USD scene graph, managing USD attributes, and manipulating the Maya runtime environment.

<SwmSnippet path="/lib/mayaUsdAPI/utils.h" line="29">

---

## getMayaRunTimeId

The `getMayaRunTimeId` function returns the currently registered Ufe runtime id for Maya.

```c
MAYAUSD_API_PUBLIC
Ufe::Rtid getMayaRunTimeId();
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/utils.h" line="33">

---

## isConnected

The `isConnected` function checks whether the two given Usd attributes are connected.

```c
MAYAUSD_API_PUBLIC
bool isConnected(const PXR_NS::UsdAttribute& srcUsdAttr, const PXR_NS::UsdAttribute& dstUsdAttr);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/utils.h" line="37">

---

## usdStage

The `usdStage` function returns the Usd stage that a given attribute belongs to.

```c
MAYAUSD_API_PUBLIC
PXR_NS::UsdStageWeakPtr usdStage(const Ufe::Attribute::Ptr& attribute);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/utils.h" line="41">

---

## usdAttributeType

The `usdAttributeType` function returns the native Usd attribute type of a given attribute.

```c
MAYAUSD_API_PUBLIC
PXR_NS::SdfValueTypeName usdAttributeType(const Ufe::Attribute::Ptr& attribute);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/utils.h" line="47">

---

## getUsdValue

The `getUsdValue` function populates the passed value argument with the Usd value stored in the attribute at the given time, returning true if it succeeds.

```c
MAYAUSD_API_PUBLIC
bool getUsdValue(
    const Ufe::Attribute::Ptr& attribute,
    PXR_NS::VtValue&           value,
    PXR_NS::UsdTimeCode        time);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
