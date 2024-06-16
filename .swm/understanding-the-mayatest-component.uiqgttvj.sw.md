---
title: Understanding the MayaTest Component
---
MayaTest in the maya-usd repository refers to a set of files and code structures used for testing purposes. It is located in the 'plugin/al/schemas/AL/usd/schemas/mayatest' directory. The files in this directory, such as 'api.h', 'tokens.h', and 'module.cpp', contain code related to the testing of the Maya schemas. These files define various tokens, APIs, and modules that are used in the testing process.

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/mayatest/api.h" line="24">

---

# MayaTest API

This is the API for MayaTest. It defines the export and import macros for the MayaTest schema.

```c
#ifndef AL_USDMAYASCHEMASTEST_API_H
#define AL_USDMAYASCHEMASTEST_API_H

#include <pxr/base/arch/export.h>

#if defined(PXR_STATIC)
#define AL_USDMAYASCHEMASTEST_API
#define AL_USDMAYASCHEMASTEST_API_TEMPLATE_CLASS(...)
#define AL_USDMAYASCHEMASTEST_API_TEMPLATE_STRUCT(...)
#define AL_USDMAYASCHEMASTEST_LOCAL
#else
#if defined(AL_USDMAYASCHEMASTEST_EXPORTS)
#define AL_USDMAYASCHEMASTEST_API                      ARCH_EXPORT
#define AL_USDMAYASCHEMASTEST_API_TEMPLATE_CLASS(...)  ARCH_EXPORT_TEMPLATE(class, __VA_ARGS__)
#define AL_USDMAYASCHEMASTEST_API_TEMPLATE_STRUCT(...) ARCH_EXPORT_TEMPLATE(struct, __VA_ARGS__)
#else
#define AL_USDMAYASCHEMASTEST_API                      ARCH_IMPORT
#define AL_USDMAYASCHEMASTEST_API_TEMPLATE_CLASS(...)  ARCH_IMPORT_TEMPLATE(class, __VA_ARGS__)
#define AL_USDMAYASCHEMASTEST_API_TEMPLATE_STRUCT(...) ARCH_IMPORT_TEMPLATE(struct, __VA_ARGS__)
#endif
#define AL_USDMAYASCHEMASTEST_LOCAL ARCH_HIDDEN
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/mayatest/tokens.h" line="24">

---

# MayaTest Tokens

This file defines the tokens for MayaTest. Tokens are used to represent property names in a more efficient manner.

```c
#ifndef AL_USDMAYASCHEMASTEST_TOKENS_H
#define AL_USDMAYASCHEMASTEST_TOKENS_H

/// \file AL_USDMayaSchemasTest/tokens.h

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//
// This is an automatically generated file (by usdGenSchema.py).
// Do not hand-edit!
//
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include "api.h"

#include <pxr/base/tf/staticData.h>
#include <pxr/base/tf/token.h>
#include <pxr/pxr.h>

#include <vector>

PXR_NAMESPACE_OPEN_SCOPE
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/mayatest/schema.usda.in" line="1">

---

# MayaTest Schema

This is the schema for MayaTest. It defines the properties of the PolyCubeNode, such as its width, height, and depth.

```in
#usda 1.0
(
    """ This file contains an example schemata for code generation using
        usdGenSchema.
    """
    subLayers = [
        @@USD_SCHEMA_PATH@@,
        @@USDGEOM_SCHEMA_PATH@@,
    ]
) 

over "GLOBAL" (
    customData = {
        string libraryName        = "AL_USDMayaSchemasTest"
        string libraryPath        = "."
        string libraryPrefix      = "AL_usd_"#This AL_ is here to get around that the Jinja template doesn't generate a template that supports namespaces. If we had a AL namespace then I could remove this AL.
        string tokensPrefix       = "AL_USDMayaSchemasTest"
    }
) {}

###################################################################################################
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/mayatest/module.cpp" line="1">

---

# MayaTest Module

This is the module for MayaTest. It wraps the PolyCubeNode class for use in Python.

```c++
#include <pxr/base/tf/pyModule.h>

PXR_NAMESPACE_USING_DIRECTIVE

TF_WRAP_MODULE { TF_WRAP(AL_usd_ExamplePolyCubeNode); }
```

---

</SwmSnippet>

# MayaTest Functions

The MayaTest functionality in the Maya-USD plugin is implemented in the AL_usd_ExamplePolyCubeNode class. This class provides functions to manipulate the attributes of a PolyCubeNode, such as its width, height, and depth.

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/mayatest/ExamplePolyCubeNode.h" line="156">

---

## GetWidthAttr Function

The `GetWidthAttr` function is used to get the width attribute of the PolyCubeNode. The width attribute represents the width of the cube.

```c
    AL_USDMAYASCHEMASTEST_API
    UsdAttribute GetWidthAttr() const;
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/mayatest/ExamplePolyCubeNode.h" line="169">

---

## GetHeightAttr Function

The `GetHeightAttr` function is used to get the height attribute of the PolyCubeNode. The height attribute represents the height of the cube.

```c
    // --------------------------------------------------------------------- //
    // HEIGHT
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/mayatest/ExamplePolyCubeNode.h" line="190">

---

## GetDepthAttr Function

The `GetDepthAttr` function is used to get the depth attribute of the PolyCubeNode. The depth attribute represents the depth of the cube.

```c
public:
    // --------------------------------------------------------------------- //
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
