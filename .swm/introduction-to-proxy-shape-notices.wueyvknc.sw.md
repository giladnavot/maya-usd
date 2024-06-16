---
title: Introduction to Proxy Shape Notices
---

Proxy Shape Notice in the maya-usd repository refers to a set of notifications related to the changes in the USD stage held by the MayaUsd proxy shape node. These notices are designed to listen to certain events that occur on the stage. They are implemented as classes inheriting from the `ProxyStageBaseNotice` class, which is a subclass of `TfNotice`.

There are three types of Proxy Shape Notices: `ProxyStageSetNotice`, `ProxyStageInvalidateNotice`, and `ProxyStageObjectsChangedNotice`. `ProxyStageSetNotice` is sent when the USD stage is set in the MayaUsd proxy shape node. `ProxyStageInvalidateNotice` is sent when the USD stage is invalidated in the MayaUsd proxy shape node. `ProxyStageObjectsChangedNotice` is sent when some objects change in the USD stage from the MayaUsd proxy shape node.

These notices are wrapped in Python using the `wrapProxyShapeNotice` function. This function defines Python bindings for the notices and their associated functions such as `resetStageSet`, `stageSet`, `resetStageInvalidated`, `stageInvalidated`, `resetStageObjectsChanged`, and `stageObjectsChanged`.

The `StageNoticesListener` class is used to register and listen to these notices. It uses the `TfNotice::Register` function to register callbacks for each notice. The `revoke_` function is used to revoke these callbacks when they are no longer needed.

<SwmSnippet path="/lib/mayaUsdAPI/proxyShapeNotice.h" line="58">

---

# ProxyStageSetNotice

`ProxyStageSetNotice` is a notice sent when the USD stage is set in the MayaUsd proxy shape node. It is used to notify other parts of the system that the stage has been set and that they should update accordingly.

```c
//! \brief Notice sent when the Usd stage is set in the mayaUsd proxy shape node
//! \class ProxyStageBaseNotice
class ProxyStageSetNotice : public ProxyStageBaseNotice
{
public:
    using ProxyStageBaseNotice::ProxyStageBaseNotice;
};
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/proxyShapeNotice.h" line="66">

---

# ProxyStageInvalidateNotice

`ProxyStageInvalidateNotice` is a notice sent when the USD stage is invalidated in the MayaUsd proxy shape node. It is used to notify other parts of the system that the stage has been invalidated and that they should update accordingly.

```c
//! \brief Notice sent when the Usd stage is invalidated in the mayaUsd proxy shape node
//! \class ProxyStageBaseNotice
class ProxyStageInvalidateNotice : public ProxyStageBaseNotice
{
public:
    using ProxyStageBaseNotice::ProxyStageBaseNotice;
};
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/proxyShapeNotice.h" line="74">

---

# ProxyStageObjectsChangedNotice

`ProxyStageObjectsChangedNotice` is a notice sent when some objects changed in the USD stage from the MayaUsd proxy shape node. It is used to notify other parts of the system that some objects in the stage have changed and that they should update accordingly.

```c
//! \brief Notice sent when some objects changed in the Usd stage from the mayaUsd proxy shape node
//! \class ProxyStageBaseNotice
class ProxyStageObjectsChangedNotice : public ProxyStageBaseNotice
{
public:
    //! Constructor
    MAYAUSD_API_PUBLIC
    ProxyStageObjectsChangedNotice(
        MObject                                  proxyObj,
        const PXR_NS::UsdNotice::ObjectsChanged& notice);

    //! Get the UsdNotice::ObjectsChanged
    MAYAUSD_API_PUBLIC
    const PXR_NS::UsdNotice::ObjectsChanged& GetNotice() const;

private:
    //! The UsdNotice::ObjectsChanged
    const PXR_NS::UsdNotice::ObjectsChanged& _notice;
};
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/python/wrapProxyShapeNotice.cpp" line="30">

---

# StageNoticesListener

`StageNoticesListener` is a class that listens to the different types of notices and triggers the corresponding updates when these notices are sent.

```c++
    StageNoticesListener()
    {
        PXR_NAMESPACE_USING_DIRECTIVE
        TfSingleton<StageNoticesListener>::SetInstanceConstructed(*this);
        TfWeakPtr<StageNoticesListener> ptr(this);
        _stageSetKey = TfNotice::Register(ptr, &StageNoticesListener::stageSet_);
        _stageInvalidatedKey = TfNotice::Register(ptr, &StageNoticesListener::stageInvalidate_);
        _stageObjectsChangedKey
            = TfNotice::Register(ptr, &StageNoticesListener::stageObjectsChanged_);
        TF_VERIFY(
            _stageSetKey.IsValid() && _stageInvalidatedKey.IsValid()
            && _stageObjectsChangedKey.IsValid());
    }
```

---

</SwmSnippet>

# Proxy Shape Notice Functions

The 'Proxy Shape Notice' is a key component in the maya-usd repository, providing a mechanism for listening to events on the USD stage. The most significant functions are found in the 'wrapProxyShapeNotice.cpp' file.

<SwmSnippet path="/lib/mayaUsdAPI/python/wrapProxyShapeNotice.cpp" line="26">

---

## StageNoticesListener

The `StageNoticesListener` class is used to listen to stage notices. It registers three types of notices: stage set, stage invalidated, and stage objects changed. Each of these notices has a corresponding function to handle the event.

```c++
// Listen to mayaUsdAPI stage notices
class StageNoticesListener : public PXR_NS::TfWeakBase
{
public:
    StageNoticesListener()
    {
        PXR_NAMESPACE_USING_DIRECTIVE
        TfSingleton<StageNoticesListener>::SetInstanceConstructed(*this);
        TfWeakPtr<StageNoticesListener> ptr(this);
        _stageSetKey = TfNotice::Register(ptr, &StageNoticesListener::stageSet_);
        _stageInvalidatedKey = TfNotice::Register(ptr, &StageNoticesListener::stageInvalidate_);
        _stageObjectsChangedKey
            = TfNotice::Register(ptr, &StageNoticesListener::stageObjectsChanged_);
        TF_VERIFY(
            _stageSetKey.IsValid() && _stageInvalidatedKey.IsValid()
            && _stageObjectsChangedKey.IsValid());
    }

    ~StageNoticesListener() { revoke_(); }

    // StageSet notice
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/python/wrapProxyShapeNotice.cpp" line="47">

---

## stageSet

The `stageSet` function is a getter for the stage set notice. It returns a boolean indicating whether the stage set notice has been triggered.

```c++
    bool stageSet() const { return _stageSet; }
    void resetStageSet() { _stageSet = false; }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/python/wrapProxyShapeNotice.cpp" line="51">

---

## stageInvalidated

The `stageInvalidated` function is a getter for the stage invalidated notice. It returns a boolean indicating whether the stage invalidated notice has been triggered.

```c++
    bool stageInvalidated() const { return _stageInvalidated; }
    void resetStageInvalidated() { _stageInvalidated = false; }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/python/wrapProxyShapeNotice.cpp" line="55">

---

## stageObjectsChanged

The `stageObjectsChanged` function is a getter for the stage objects changed notice. It returns a boolean indicating whether the stage objects changed notice has been triggered.

```c++
    bool stageObjectsChanged() const { return _stageObjectsChanged; }
    void resetStageObjectsChanged() { _stageObjectsChanged = false; }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/python/wrapProxyShapeNotice.cpp" line="69">

---

## revoke\_

The `revoke_` function is used to revoke the registration of the notices. If the key of a notice is valid, the registration of the notice is revoked.

```c++
    void revoke_()
    {
        if (_stageSetKey.IsValid()) {
            PXR_NS::TfNotice::Revoke(_stageSetKey);
        }
        if (_stageInvalidatedKey.IsValid()) {
            PXR_NS::TfNotice::Revoke(_stageInvalidatedKey);
        }
        if (_stageObjectsChangedKey.IsValid()) {
            PXR_NS::TfNotice::Revoke(_stageObjectsChangedKey);
        }
    }
```

---

</SwmSnippet>

# ProxyStage and ProxyStageBaseNotice Overview

Understanding ProxyStage and ProxyStageBaseNotice

<SwmSnippet path="/lib/mayaUsdAPI/proxyStage.h" line="33">

---

## ProxyStage

The `ProxyStage` class is an accessor for a UsdTime and a UsdStageRefPtr from a MObject which comes from a mayaUsd ProxyStageProvider subclass. It provides methods to get the UsdTimeCode (`getTime`) and the UsdStageRefPtr (`getUsdStage`) from the ProxyStage.

```c
/*! \brief  Class to access Usd time and Usd stage from a maya MObject which comes from a mayaUsd
   ProxyStageProvider subclass (usually a MayaUsdProxyShapeBase node)
   \class  ProxyStage
   This class is an accessor for a UsdTime and a UsdStageRefPtr from a MObject which comes from a
   mayaUsd ProxyStageProvider subclass. Usually a MayaUsdProxyShapeBase node.
   Example usage: \code
    {
        MObject dagNode;//is the MObject of a MayaUsdProxyShape node for example
        MayaUsdAPI::ProxyStage proxyStage(dagNode);
        auto stage = proxyStage.getUsdStage();
        auto time = proxyStage.getTime();
    }
    \endcode
*/
class MAYAUSD_API_PUBLIC ProxyStage
{
public:
    /*! Constructor from an MObject whose MPxNode must be a subclass of mayaUsd
     * ProxyStageProvider, such as a MayaUsdProxyShapeBase node
     * @param[in] obj is a MObject from a subclass of mayaUsd ProxyStageProvider, such as a
     * MayaUsdProxyShapeBase node
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsdAPI/proxyShapeNotice.h" line="34">

---

## ProxyStageBaseNotice

The `ProxyStageBaseNotice` class is the base class for all Notices related to Usd stages events. It provides methods to get the proxy shape MObject (`GetProxyShapeObj`), the proxy shape Maya::MDagPath as a string (`GetProxyShapePath`), and the usd stage (`GetProxyShapeStage`).

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
