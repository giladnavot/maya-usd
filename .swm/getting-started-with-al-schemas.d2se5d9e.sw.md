---
title: Getting Started with al Schemas
---
The 'al Schemas' in the maya-usd repository refers to a set of schemas defined under the 'plugin/al/schemas' directory. These schemas are used to define the structure and data types of the USD (Universal Scene Description) files used in the plugin. They are written in both Python and C++, and include definitions for various types of data, such as ModelAPI and FrameRange. The schemas are used to ensure consistency and integrity of the data used in the plugin.

# al Schemas Directory Structure

The 'al Schemas' are organized in a directory structure under 'plugin/al/schemas'. Each schema is defined in its own directory, such as 'maya' or 'mayatest'.

<SwmSnippet path="/plugin/al/schemas/usd/schema.usda" line="1">

---

# Schema Definition

Each 'al Schema' is defined in a '.usda' file. This file describes the USD primitive schemata and drives code generation. It defines the properties and behaviors of the schema.

```usda
#usda 1.0
(
    "This file describes the USD primitive schemata and drives code generation."
)

def "GLOBAL" (
    customData = {
        string libraryName      = "usd"
        string libraryPath      = "pxr/usd/usd"
        # string libraryPrefix  = "Usd"
        # string tokensPrefix   = "Usd"
        dictionary libraryTokens = {
            dictionary clipAssetPaths = {
              string doc = """
              List of asset paths to the clips for this prim. This list is
              unordered, but elements in this list are referred to by index in
              other clip-related fields.
              """
            }

            dictionary clipPrimPath = {
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/ModelAPI.cpp" line="27">

---

# Schema Registration

Each 'al Schema' needs to be registered with the TfType system. This is done in the schema's implementation file using the 'TF_REGISTRY_FUNCTION' macro.

```c++
TF_REGISTRY_FUNCTION(TfType) { TfType::Define<AL_usd_ModelAPI, TfType::Bases<UsdModelAPI>>(); }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/__init__.py" line="3">

---

# Schema Usage

The 'al Schemas' can be used in Python scripts by importing the '\_AL_USDMayaSchemas' module. This module prepares the 'al Schemas' for use in Python.

```python
    Tf.PreparePythonModule('_AL_USDMayaSchemas')
else:
    from . import _AL_USDMayaSchemas
    Tf.PrepareModule(_AL_USDMayaSchemas, locals())
del Tf
```

---

</SwmSnippet>

# AL Schemas Endpoints

AL Schemas Endpoints

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/FrameRange.h" line="91">

---

## AL_usd_FrameRange::Get

The `Get` method of the `AL_usd_FrameRange` class returns an `AL_usd_FrameRange` object holding the prim adhering to this schema at a given path on a given stage. If no prim exists at the path on the stage, or if the prim at that path does not adhere to this schema, it returns an invalid schema object.

```c
    AL_USDMAYASCHEMAS_API
    static AL_usd_FrameRange Get(const UsdStagePtr& stage, const SdfPath& path);
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/ModelAPI.h" line="91">

---

## AL_usd_ModelAPI::Get

The `Get` method of the `AL_usd_ModelAPI` class returns an `AL_usd_ModelAPI` object holding the prim adhering to this schema at a given path on a given stage. If no prim exists at the path on the stage, or if the prim at that path does not adhere to this schema, it returns an invalid schema object.

```c
    AL_USDMAYASCHEMAS_API
    static AL_usd_ModelAPI Get(const UsdStagePtr& stage, const SdfPath& path);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
