---
title: Overview of MayaUsdAPI
---

MayaUsdAPI is a part of the Maya-USD plugin that provides a set of APIs for interacting with USD data within Maya. It includes a set of headers and source files that define various functionalities such as versioning, namespace management, and utility functions. The API is designed to be used by other parts of the Maya-USD plugin and potentially by third-party plugins that want to leverage USD capabilities within Maya.

The MayaUsdAPI is defined in a set of header files, including `mayaUsdAPI.h.src` and `api.h`. These files define various macros and namespaces used throughout the API. For example, `mayaUsdAPI.h.src` defines the versioning scheme for the API, while `api.h` includes export symbols for the library.

The API also includes utility functions, defined in `utils.h` and `utils.cpp`, which provide useful operations that can be used by other parts of the plugin. For example, `getMayaRunTimeId` is a function that returns the currently registered Ufe runtime id for Maya.

In addition to the C++ API, MayaUsdAPI also provides Python bindings. These are defined in the `python` directory, which includes a set of wrapper classes and functions that expose the C++ API to Python. This allows scripts and plugins written in Python to interact with USD data within Maya.

<SwmSnippet path="/lib/mayaUsdAPI/mayaUsdAPI.h.src" line="20">

---

# MayaUsdAPI Versioning

The MayaUsdAPI uses a versioning system, defined by the `MAYAUSDAPI_MAJOR_VERSION`, `MAYAUSDAPI_MINOR_VERSION`, and `MAYAUSDAPI_PATCH_LEVEL` macros. This allows for backward compatibility and flexibility in API development.

```src
#define MAYAUSDAPI_MAJOR_VERSION ${MAYAUSDAPI_MAJOR_VERSION}
#define MAYAUSDAPI_MINOR_VERSION ${MAYAUSDAPI_MINOR_VERSION}
#define MAYAUSDAPI_PATCH_LEVEL ${MAYAUSDAPI_PATCH_LEVEL}
#define MAYAUSDAPI_API_VERSION (MAYAUSDAPI_MAJOR_VERSION * 10000 + MAYAUSDAPI_MINOR_VERSION * 100 + MAYAUSDAPI_PATCH_LEVEL)

// mayaUsdAPI viewport public namespace string will never change.
#define MAYAUSDAPI_NS MayaUsdAPI
// C preprocessor trickery to expand arguments.
#define MAYAUSDAPI_CONCAT(A, B) MAYAUSDAPI_CONCAT_IMPL(A, B)
#define MAYAUSDAPI_CONCAT_IMPL(A, B) A##B
// Versioned namespace includes the major version number.
#define MAYAUSDAPI_VERSIONED_NS MAYAUSDAPI_CONCAT(MAYAUSDAPI_NS, _v${MAYAUSDAPI_MAJOR_VERSION})

namespace MAYAUSDAPI_VERSIONED_NS {}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/mayaUsdAPI.h.src" line="26">

---

# MayaUsdAPI Namespace

The MayaUsdAPI uses a namespace defined by the `MAYAUSDAPI_NS` macro. This namespace is versioned, allowing for different versions of the API to coexist without conflict.

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
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/python/__init__.py" line="3">

---

# MayaUsdAPI Python Module

The MayaUsdAPI also provides a Python module, `_mayaUsdAPI`, which can be imported and used in Python scripts to interact with USD data.

```python
    Tf.PreparePythonModule('_mayaUsdAPI')
else:
    from . import _mayaUsdAPI
    Tf.PrepareModule(_mayaUsdAPI, locals())
    del _mayaUsdAPI
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/utils.h" line="26">

---

# MayaUsdAPI Utilities

The MayaUsdAPI provides a set of utilities, defined in the `MAYAUSDAPI_NS_DEF` namespace, that provide additional functionality for interacting with USD data.

```c
namespace MAYAUSDAPI_NS_DEF {

//! Returns the currently registered Ufe runtime id for Maya.
```

---

</SwmSnippet>

# Functions of MayaUsdAPI

This section will cover the main functions of the MayaUsdAPI, including getMayaRunTimeId, getUsdValue, addNewMaterialCommand, and the ProxyStage class.

<SwmSnippet path="/lib/mayaUsdAPI/utils.cpp" line="33">

---

## getMayaRunTimeId

The `getMayaRunTimeId` function is used to get the runtime ID for Maya. This ID is used to identify the Maya runtime in the context of Universal Scene Description (USD).

```c++
Ufe::Rtid getMayaRunTimeId() { return MayaUsd::ufe::getMayaRunTimeId(); }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/utils.cpp" line="56">

---

## getUsdValue

The `getUsdValue` function is used to get the value of a USD attribute. It takes a UFE attribute and a time code as input and retrieves the corresponding value from the USD stage.

```c++
bool getUsdValue(
    const Ufe::Attribute::Ptr& attribute,
    PXR_NS::VtValue&           value,
    PXR_NS::UsdTimeCode        time)
{
    if (auto usdAttribute = std::dynamic_pointer_cast<UsdUfe::UsdAttribute>(attribute)) {
        return usdAttribute->get(value, time);
    }
    return false;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/utils.cpp" line="69">

---

## addNewMaterialCommand

The `addNewMaterialCommand` function is used to create a new material in the USD stage. It takes a scene item and a shader identifier as input and returns a command that can be executed to add the new material.

```c++
Ufe::UndoableCommand::Ptr
addNewMaterialCommand(const Ufe::SceneItem::Ptr& parentItem, const std::string& sdrShaderIdentifier)
{
    if (auto usdSceneItem = UsdUfe::downcast(parentItem)) {
        return MayaUsd::ufe::UsdUndoAddNewMaterialCommand::create(
            usdSceneItem, sdrShaderIdentifier);
    }
    return nullptr;
}

Ufe::UndoableCommand::Ptr createMaterialsScopeCommand(const Ufe::SceneItem::Ptr& parentItem)
{
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/proxyStage.h" line="46">

---

## ProxyStage

The `ProxyStage` class is used to interact with the USD stage associated with a Maya proxy shape. It provides methods to get the current time and the USD stage. It is constructed with a Maya object that must be a subclass of the ProxyStageProvider, such as a MayaUsdProxyShapeBase node.

```c
*/
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
```

---

</SwmSnippet>

# MayaUsdAPI Endpoints

Overview of MayaUsdAPI Endpoints

<SwmSnippet path="/lib/mayaUsdAPI/proxyStage.h" line="47">

---

## ProxyStage

The `ProxyStage` class is an accessor for a UsdTime and a UsdStageRefPtr from a MObject which comes from a mayaUsd ProxyStageProvider subclass. It provides methods to get the UsdTimeCode and the UsdStageRefPtr.

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

<SwmSnippet path="/lib/mayaUsdAPI/proxyStage.h" line="70">

---

## getUsdStage

The `getUsdStage` method of the `ProxyStage` class returns a UsdStageRefPtr. If the ProxyStage is not valid, it returns nullptr.

```c
    PXR_NS::UsdStageRefPtr getUsdStage() const;
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
