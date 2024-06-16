---
title: Understanding USD Maya Core Libraries
---

The USD Maya Core Libraries in the maya-usd repository refer to a set of Python modules and MEL scripts that provide the functionality for the Maya plugin for Pixar's Universal Scene Description (USD). These libraries include modules for reading and writing USD data, handling stage caching, and various utilities for round-trip conversions between Maya and USD. They also include MEL scripts for creating USD proxy shapes and managing assembly variants. These libraries are essential for the operation of the Maya plugin, enabling it to interact with USD data and provide a seamless user experience.

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/__init__.py" line="51">

---

## Using ReadUtil

`ReadUtil` is a module in the USD Maya Core Libraries that provides functionalities for reading USD data.

```python
    from mayaUsd.lib import ReadUtil
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/__init__.py" line="57">

---

## Using WriteUtil

`WriteUtil` is another module in the USD Maya Core Libraries that provides functionalities for writing USD data.

```python
    from mayaUsd.lib import WriteUtil
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/__init__.py" line="50">

---

## Using MeshWriteUtils

`MeshWriteUtils` is a module in the USD Maya Core Libraries that provides functionalities for writing mesh data specifically.

```python
    from mayaUsd.lib import MeshWriteUtils
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/__init__.py" line="54">

---

## Using StageCache

`StageCache` is a module in the USD Maya Core Libraries that provides functionalities for caching stages for better performance.

```python
    from mayaUsd.lib import StageCache
```

---

</SwmSnippet>

# USD Maya Core Libraries Functions

This section provides an overview of the main functions in the USD Maya Core Libraries.

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/referenceAssembly.h" line="66">

---

## UsdMayaReferenceAssembly

The 'UsdMayaReferenceAssembly' class is a key component of the USD Maya Core Libraries. It provides the core functionality for importing and exporting between USD data in Maya. It also contains the logic for USD assembly references, proxy shapes, and related Maya nodes.

```c
class UsdMayaReferenceAssembly
    : public MPxAssembly
    , public UsdMayaUsdPrimProvider
{
public:
    PXRUSDMAYA_API
    static const MTypeId typeId;
    PXRUSDMAYA_API
    static const MString typeName;
    PXRUSDMAYA_API
    static const MString _classification;

    // Attributes
    PXRUSDMAYA_API
    static MObject filePathAttr;
    PXRUSDMAYA_API
    static MObject primPathAttr;
    PXRUSDMAYA_API
    static MObject excludePrimPathsAttr;
    PXRUSDMAYA_API
    static MObject timeAttr;
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/referenceAssembly.h" line="406">

---

## UsdMayaRepresentationHierBase

The 'UsdMayaRepresentationHierBase' class is a base class for representations that unroll a hierarchy. It provides the functionality to connect sub-assembly plugs and proxy plugs.

```c
class UsdMayaRepresentationHierBase : public UsdMayaRepresentationBase
{
public:
    // == Overrides for MPxRepresentation ==
    UsdMayaRepresentationHierBase(MPxAssembly* assembly, const MString& name)
        : UsdMayaRepresentationBase(assembly, name) {};

    PXRUSDMAYA_API
    bool activate() override;

protected:
    PXRUSDMAYA_API
    void _ConnectSubAssemblyPlugs();
    PXRUSDMAYA_API
    void _ConnectProxyPlugs();

    virtual bool _ShouldImportWithProxies() const { return false; };
};
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/referenceAssembly.h" line="431">

---

## UsdMayaRepresentationExpanded

The 'UsdMayaRepresentationExpanded' class is used to expand a USD hierarchy into sub-assemblies. It imports transforms as Maya groups and other prims as usdPrimShapes. Children that are models, model groups, and sets will be imported as UsdAssemblies.

```c
class UsdMayaRepresentationExpanded : public UsdMayaRepresentationHierBase
{
public:
    // == Statics
    PXRUSDMAYA_API
    static const MString _assemblyType;

    // == Overrides for MPxRepresentation ==
    UsdMayaRepresentationExpanded(MPxAssembly* assembly, const MString& name)
        : UsdMayaRepresentationHierBase(assembly, name) {};

    MString getType() const override { return UsdMayaRepresentationExpanded::_assemblyType; };

protected:
    bool _ShouldImportWithProxies() const override { return true; };
};
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/referenceAssembly.h" line="452">

---

## UsdMayaRepresentationFull

The 'UsdMayaRepresentationFull' class is used to import the USD subgraph as full Maya geometry. It provides the functionality to import and export Maya shading networks to USD shading.

```c
//
class UsdMayaRepresentationFull : public UsdMayaRepresentationHierBase
{
public:
    // == Statics
    PXRUSDMAYA_API
    static const MString _assemblyType;

    // == Overrides for MPxRepresentation ==
    UsdMayaRepresentationFull(MPxAssembly* assembly, const MString& name)
        : UsdMayaRepresentationHierBase(assembly, name) {};

    MString getType() const override { return UsdMayaRepresentationFull::_assemblyType; };
};
```

---

</SwmSnippet>

# USD Maya Core Libraries

Understanding USD Maya Core Libraries

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/referenceAssembly.h" line="143">

---

## UsdMayaReferenceAssembly::postLoadPath

The `postLoadPath` method of the `UsdMayaReferenceAssembly` class is used after a file path is loaded. It's responsible for handling the loaded path and performing necessary operations such as setting the prim path and variant selection.

```c
    PXRUSDMAYA_API
    MStatus deleteAllRepresentations() override;
    PXRUSDMAYA_API
    MString setRepName(
        const MString& representation,
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/editUtil.h" line="94">

---

## UsdMayaEditUtil::GetEditFromString

The `GetEditFromString` method of the `UsdMayaEditUtil` class is used to translate an edit string into a `AssemblyEdit` structure. It takes an assembly function set, an edit string, and output parameters for the edit path and the edit itself. It returns true if the translation was successful.

```c
    static bool GetEditFromString(
        const MFnAssembly& assemblyFn,
        const std::string& editString,
        SdfPath*           outEditPath,
        AssemblyEdit*      outEdit);
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
