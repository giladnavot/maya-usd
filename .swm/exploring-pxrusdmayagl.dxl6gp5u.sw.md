---
title: Exploring PxrUsdMayaGL
---
PxrUsdMayaGL is a part of the Maya to USD interface, specifically handling the rendering aspects. It contains classes and functions that manage the translation of Maya shape node data and viewport state for imaging with Hydra. This includes managing the state of the viewport, handling soft selection, and tracking instancers. It also provides debugging capabilities.

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/userData.h" line="33">

---

## PxrMayaHdUserData Class

The `PxrMayaHdUserData` class is a container for all of the information needed for a draw request in the legacy viewport or Viewport 2.0, without requiring shape querying at draw time. It inherits from the `MUserData` class and includes a bounding box and a wireframe color.

```c
/// Container for all of the information needed for a draw request in the
/// legacy viewport or Viewport 2.0, without requiring shape querying at draw
/// time.
///
/// Maya shapes may implement their own derived classes of this class if they
/// require storage for additional data that's not specific to the batch
/// renderer.
class PxrMayaHdUserData : public MUserData
{
public:
    std::unique_ptr<MBoundingBox> boundingBox;
    std::unique_ptr<GfVec4f>      wireframeColor;

    MAYAUSD_CORE_PUBLIC
    PxrMayaHdUserData();

    MAYAUSD_CORE_PUBLIC
    ~PxrMayaHdUserData() override;
};
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/instancerImager.h" line="37">

---

## UsdMayaGL_InstancerImager Class

The `UsdMayaGL_InstancerImager` class is used for syncing native Maya instancers with the pxrHdImagingShape so that it can draw USD reference assemblies connected to native Maya instancers.

```c
/// Class for syncing native Maya instancers with the pxrHdImagingShape so that
/// it can draw USD reference assemblies connected to native Maya instancers.
///
/// XXX We currently don't support drawing multiple instanced instancers.
/// When instancer nodes appear at multiple points in the DAG path via native
/// Maya instancing, we only draw the 0th instance. This behavior is similar to
/// the current imaging behavior of USD proxy shapes, where only one instance
/// gets drawn by the draw override.
class UsdMayaGL_InstancerImager : public TfWeakBase
{
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/shapeAdapter.h" line="51">

---

## PxrMayaHdShapeAdapter Class

The `PxrMayaHdShapeAdapter` class is an abstract base class for objects that manage translation of Maya shape node data and viewport state for imaging with Hydra. It provides methods to update the shape adapter's state, get the Maya user data object for drawing in the legacy viewport and Viewport 2.0, and get the render params for the shape adapter's current state.

```c
/// Abstract base class for objects that manage translation of Maya shape node
/// data and viewport state for imaging with Hydra.
class PxrMayaHdShapeAdapter
{
public:
    /// Update the shape adapter's state from the shape with the given
```

---

</SwmSnippet>

# PxrUsdMayaGL Functions

This section covers the main functions of PxrUsdMayaGL in the maya-usd repository.

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/instancerImager.cpp" line="43">

---

## GetInstance

The `GetInstance` function is a static method that returns an instance of `UsdMayaGL_InstancerImager`. It uses the singleton pattern to ensure that only one instance of `UsdMayaGL_InstancerImager` exists.

```c++
/* static */
UsdMayaGL_InstancerImager& UsdMayaGL_InstancerImager::GetInstance()
{
    return TfSingleton<UsdMayaGL_InstancerImager>::GetInstance();
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/instancerImager.cpp" line="276">

---

## \_OnWorldMatrixChanged

The `_OnWorldMatrixChanged` function is a static method that is triggered when the world matrix of a transform node changes. It updates the transformation of the instancer and marks the HdImagingShape as dirty if the transformation has changed.

```c++
/* static */
void UsdMayaGL_InstancerImager::_OnWorldMatrixChanged(
    MObject&                          transformNode,
    MDagMessage::MatrixModifiedFlags& modified,
    void*                             clientData)
{
    UsdMayaGL_InstancerImager& me = GetInstance();
    const auto                 handleEntryPair
        = static_cast<std::pair<MObjectHandle, _InstancerEntry>*>(clientData);
    const MObjectHandle& handle = handleEntryPair->first;

    bool inserted = false;
    inserted |= me._dirtyInstancerXformsVp2.insert(handle).second;
    inserted |= me._dirtyInstancerXformsLegacy.insert(handle).second;
    if (inserted) {
        me._DirtyHdImagingShape(false);
    }
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/instancerImager.cpp" line="301">

---

## \_OnConnection

The `_OnConnection` function is triggered when a connection is made to an instancer. It checks if the instancer is in interactive mode, and if so, it adds the instancer to the tracking list and starts tracking it.

```c++
void UsdMayaGL_InstancerImager::_OnConnection(
    const UsdMayaAssemblyConnectedToInstancerNotice& notice)
{
    if (MGlobal::mayaState() != MGlobal::kInteractive) {
        return;
    }

    MObject             instancer = notice.GetInstancer();
    const MObjectHandle instancerHandle(instancer);

    // Remove the instancer from the removal list, if it was previously
    // scheduled for removal.
    _instancersToRemove.erase(instancerHandle);

    // Create a new entry in our instancers list only if we haven't seen this
    // instancer before.
    if (_instancers.count(instancerHandle) == 0) {
        _StartTrackingInstancer(instancer);
    }
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/instancerImager.cpp" line="322">

---

## \_OnDisconnection

The `_OnDisconnection` function is triggered when a disconnection is made from an instancer. It checks if the instancer is in interactive mode, and if so, it removes the instancer from the tracking list and stops tracking it.

```c++
void UsdMayaGL_InstancerImager::_OnDisconnection(
    const UsdMayaAssemblyDisconnectedFromInstancerNotice& notice)
{
    if (MGlobal::mayaState() != MGlobal::kInteractive) {
        return;
    }

    MObject             instancer = notice.GetInstancer();
    const MObjectHandle instancerHandle(instancer);

    MStatus           status;
    MFnDependencyNode instancerDepNode(instancer, &status);
    if (!status) {
        return;
    }

    MPlug inputHierarchy = instancerDepNode.findPlug("inputHierarchy", &status);
    if (!status) {
        return;
    }

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/instancerImager.cpp" line="373">

---

## UsdMayaGL_InstancerImager

The `UsdMayaGL_InstancerImager` constructor initializes the object and registers it to receive notifications when the scene is reset, or when a connection or disconnection is made.

```c++
UsdMayaGL_InstancerImager::UsdMayaGL_InstancerImager()
{
    TfWeakPtr<UsdMayaGL_InstancerImager> me(this);
    TfNotice::Register(me, &UsdMayaGL_InstancerImager::_OnSceneReset);
    TfNotice::Register(me, &UsdMayaGL_InstancerImager::_OnConnection);
    TfNotice::Register(me, &UsdMayaGL_InstancerImager::_OnDisconnection);
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/instancerImager.cpp" line="410">

---

## CreateInstancerShapeAdapter

The `CreateInstancerShapeAdapter` function creates a new `UsdMayaGL_InstancerShapeAdapter` object. If a factory for creating shape adapters is set, it uses the factory to create the shape adapter.

```c++
UsdMayaGL_InstancerShapeAdapter*
UsdMayaGL_InstancerImager::CreateInstancerShapeAdapter(bool isViewport2)
{
    if (!_instancerShapeAdapterFactory) {
        return new UsdMayaGL_InstancerShapeAdapter(isViewport2);
    }
    return _instancerShapeAdapterFactory(isViewport2);
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
