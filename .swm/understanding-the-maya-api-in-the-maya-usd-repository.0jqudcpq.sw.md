---
title: Understanding the Maya API in the maya-usd Repository
---

The Maya API in the maya-usd repository refers to a set of macros and definitions that are used to create a bridge between the Maya software and the Universal Scene Description (USD) system developed by Pixar. This API is defined in the `lib/mayaUsdAPI/api.h` file and includes macros such as `MAYAUSD_API_H`, `MAYAUSD_API_PUBLIC`, and `MAYAUSD_API_LOCAL`. These macros are used to control the visibility of the API's functions and classes when they are exported to a dynamic library. The API also includes versioning information, which is defined in the `lib/mayaUsdAPI/mayaUsdAPI.h.src` file. This versioning information is used to ensure compatibility between different versions of the API.

<SwmSnippet path="/lib/mayaUsdAPI/api.h" line="17">

---

## Macro Definitions

The 'MAYAUSD_API_H' macro is defined in this file. This macro is used to prevent the header file from being included more than once in the same source file.

```c
#ifndef MAYAUSD_API_H
#define MAYAUSD_API_H

#include <pxr/base/arch/export.h>
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/api.h" line="27">

---

The 'MAYAUSD_API_PUBLIC' macro is defined here. This macro is used to manage the visibility of the functions and classes. It is defined differently based on whether 'MAYAUSD_API_EXPORT' is defined and the compiler being used.

```c
  #ifdef MAYAUSD_API_EXPORT
    #ifdef __GNUC__
      #define MAYAUSD_API_PUBLIC __attribute__ ((dllexport))
    #else
      #define MAYAUSD_API_PUBLIC __declspec(dllexport)
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/mayaUsdAPI.h.src" line="26">

---

## Namespace Definitions

The 'MAYAUSDAPI_NS' and 'MAYAUSDAPI_VERSIONED_NS' namespaces are defined in this file. These namespaces are used to manage the versioning of the API.

```src
#define MAYAUSDAPI_NS MayaUsdAPI
// C preprocessor trickery to expand arguments.
#define MAYAUSDAPI_CONCAT(A, B) MAYAUSDAPI_CONCAT_IMPL(A, B)
#define MAYAUSDAPI_CONCAT_IMPL(A, B) A##B
// Versioned namespace includes the major version number.
#define MAYAUSDAPI_VERSIONED_NS MAYAUSDAPI_CONCAT(MAYAUSDAPI_NS, _v${MAYAUSDAPI_MAJOR_VERSION})

namespace MAYAUSDAPI_VERSIONED_NS {}

// With a using namespace declaration, pull in the versioned namespace into the
// mayaUsdAPI public namespace, to allow client code to use the plain mayaUsdAPI
// namespace, e.g. MayaUsdAPI::Class.
namespace MAYAUSDAPI_NS {
using namespace MAYAUSDAPI_VERSIONED_NS;
}

// Macro to place the mayaUsdAPI symbols in the versioned namespace, which
// is how they will appear in the shared library, e.g. MayaUsdAPI_v1::Class.
#ifdef DOXYGEN
#define MAYAUSDAPI_NS_DEF MAYAUSDAPI_NS
#else
```

---

</SwmSnippet>

## Python Bindings

The 'mayaUsdAPI/python' directory contains the Python bindings for the Maya API. These bindings allow the Maya API to be used from Python scripts.

# Maya API Endpoints

Exploring the Maya API

<SwmSnippet path="/lib/mayaUsdAPI/proxyStage.h" line="47">

---

## ProxyStage

The `ProxyStage` class is an accessor for a UsdTime and a UsdStageRefPtr from a MObject which comes from a mayaUsd ProxyStageProvider subclass. It provides methods to get the UsdTimeCode and the UsdStageRefPtr. It is typically used with a MayaUsdProxyShapeBase node.

```c
class MAYAUSD_API_PUBLIC ProxyStage
{
public:
    /*! Constructor from an MObject whose MPxNode must be a subclass of mayaUsd
     * ProxyStageProvider, such as a MayaUsdProxyShapeBase node
     * @param[in] obj is a MObject from a subclass of mayaUsd ProxyStageProvider, such as a
     * MayaUsdProxyShapeBase node
     */
    ProxyStage(const MObject& obj);
    /*! Copy constructor
     * @param[in] other is another ProxyStage
     */
    ProxyStage(const ProxyStage& other);
    //! Destructor
    ~ProxyStage();
    /*! Returns a UsdTimeCode
     *  \return a UsdTimeCode, if the ProxyStage is not valid, it returns a default constructed
     * UsdTimeCode()
     */
    PXR_NS::UsdTimeCode getTime() const;
    /*! Returns a UsdStageRefPtr
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/proxyShapeNotice.h" line="34">

---

## ProxyStageBaseNotice

The `ProxyStageBaseNotice` class is a base class for all Notices related to Usd stages events. It provides methods to get the proxy shape MObject, the proxy shape Maya::MDagPath as a string, and the usd stage.

```c
class ProxyStageBaseNotice : public PXR_NS::TfNotice
{
public:
    //! Constructor
    MAYAUSD_API_PUBLIC
    ProxyStageBaseNotice(MObject proxyObj);

    //! Get the proxy shape MObject
    MAYAUSD_API_PUBLIC
    MObject GetProxyShapeObj() const;

    //! Get the proxy shape Maya::MDagPath as a string
    MAYAUSD_API_PUBLIC
    std::string GetProxyShapePath() const;

    //! Get the usd stage
    MAYAUSD_API_PUBLIC
    PXR_NS::UsdStageRefPtr GetProxyShapeStage() const;

private:
    //! The mayaUsd proxy shape MObject
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
