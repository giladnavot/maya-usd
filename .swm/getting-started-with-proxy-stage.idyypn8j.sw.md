---
title: Getting Started with Proxy Stage
---

The ProxyStage in the maya-usd repository is a class that provides an interface to interact with the USD stage associated with a Maya proxy shape node. It is designed to be used in conjunction with the ProxyStageProvider interface, which is implemented by the MayaUsdProxyShapeBase class. The ProxyStage class uses a private implementation pattern to hide the details of its implementation and to allow for future changes without breaking binary compatibility. The ProxyStage class provides methods to retrieve the USD stage and the current time of the proxy shape node.

<SwmSnippet path="/lib/mayaUsdAPI/proxyStage.cpp" line="62">

---

# ProxyStage Class

Here is the definition of the ProxyStage class. It is constructed with another ProxyStage or an MObject. The constructor initializes a ProxyStageImp instance, which encapsulates the ProxyStageProvider.

```c++
ProxyStage::ProxyStage(const ProxyStage& other)
    : _imp(new ProxyStageImp(other._imp->_proxyStageProvider))
{
    PXR_NAMESPACE_USING_DIRECTIVE
    TF_AXIOM(other._imp->_proxyStageProvider);
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/proxyStage.cpp" line="25">

---

# ProxyStageImp Struct

The ProxyStageImp struct is a helper structure used within the ProxyStage class. It is responsible for holding the ProxyStageProvider and ensuring it is of the correct type.

```c++
struct ProxyStageImp
{
    ProxyStageImp(MPxNode* node)
        : _proxyStageProvider(dynamic_cast<PXR_NS::ProxyStageProvider*>(node))
    {

        PXR_NAMESPACE_USING_DIRECTIVE
        const std::string errMsg = TfStringPrintf(
            "The node passed to the constructor of ProxyStage is not a "
            "MayaUsdProxyShapeBase subclass node while it should ! Its type is : %s",
            node->typeName().asChar());

        TF_VERIFY(_proxyStageProvider, "%s", errMsg.c_str());
        if (!_proxyStageProvider) {
            throw std::runtime_error(errMsg);
        }
    }
    ProxyStageImp(PXR_NS::ProxyStageProvider* proxyStageProvider)
        : _proxyStageProvider(proxyStageProvider)
    {
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/proxyStage.cpp" line="73">

---

# getUsdStage Method

The getUsdStage method is a member of the ProxyStage class. It retrieves the USD stage from the encapsulated ProxyStageProvider.

```c++
PXR_NS::UsdStageRefPtr ProxyStage::getUsdStage() const
{
    return _imp->_proxyStageProvider->getUsdStage();
}
```

---

</SwmSnippet>

# Proxy Stage Functions

This section provides an overview of the main functions associated with the Proxy Stage in Maya-USD.

<SwmSnippet path="/lib/mayaUsdAPI/python/wrapProxyShapeNotice.cpp" line="44">

---

## stageSet and resetStageSet

The `stageSet` function checks if the stage is set, returning a boolean value. The `resetStageSet` function is used to reset the stage set status to false.

```c++
    ~StageNoticesListener() { revoke_(); }

    // StageSet notice
    bool stageSet() const { return _stageSet; }
    void resetStageSet() { _stageSet = false; }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/python/wrapProxyShapeNotice.cpp" line="48">

---

## stageInvalidated and resetStageInvalidated

The `stageInvalidated` function checks if the stage is invalidated, returning a boolean value. The `resetStageInvalidated` function is used to reset the stage invalidated status to false.

```c++
    void resetStageSet() { _stageSet = false; }

    // StageInvalidated notice
    bool stageInvalidated() const { return _stageInvalidated; }
    void resetStageInvalidated() { _stageInvalidated = false; }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/python/wrapProxyShapeNotice.cpp" line="52">

---

## stageObjectsChanged and resetStageObjectsChanged

The `stageObjectsChanged` function checks if the objects in the stage have changed, returning a boolean value. The `resetStageObjectsChanged` function is used to reset the stage objects changed status to false.

```c++
    void resetStageInvalidated() { _stageInvalidated = false; }

    // StageObjectsChanged notice
    bool stageObjectsChanged() const { return _stageObjectsChanged; }
    void resetStageObjectsChanged() { _stageObjectsChanged = false; }
```

---

</SwmSnippet>

# Proxy Stage Endpoints

Understanding Proxy Stage Endpoints

<SwmSnippet path="/lib/mayaUsdAPI/proxyStage.h" line="47">

---

## ProxyStage

The `ProxyStage` class provides access to the USD stage and time from a Maya object. It is typically used with a MayaUsdProxyShapeBase node. The class provides methods to get the USD stage (`getUsdStage`) and the time (`getTime`).

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

The `ProxyStageBaseNotice` class is a base class for all notices related to USD stage events. It provides methods to get the proxy shape object (`GetProxyShapeObj`), the proxy shape path (`GetProxyShapePath`), and the USD stage (`GetProxyShapeStage`).

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
