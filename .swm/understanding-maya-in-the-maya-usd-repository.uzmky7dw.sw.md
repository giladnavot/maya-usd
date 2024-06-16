---
title: Understanding Maya in the maya-usd Repository
---
Maya, in the context of the maya-usd repository, refers to the 3D computer graphics application from Autodesk. The repository provides a plugin for Maya that allows it to work with Pixar's Universal Scene Description (USD). The codebase contains several references to Maya, such as 'Maya Frame Range', 'start animation frame in Maya', and 'min frame in Maya', which are related to the animation frames in the Maya application. The repository also includes schemas for storing the lock state of corresponding Maya objects of the prims and the state which makes transform attributes of Maya objects (including children) locked.

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/tests/testMayaSchemas.cpp" line="1">

---

# Maya in the Code

This test file demonstrates how Maya is used in the repository. It includes tests for Maya schemas and references, which are crucial for integrating Maya with USD.

```c++
#include <mayaUsd_Schemas/MayaReference.h>

#include <pxr/base/plug/plugin.h>
#include <pxr/base/plug/registry.h>
#include <pxr/pxr.h>
#include <pxr/usd/usd/stage.h>

#include <gtest/gtest.h>

#include <functional>
#include <iostream>

PXR_NAMESPACE_USING_DIRECTIVE
// TF_DECLARE_WEAK_PTRS(PlugPlugin);

/// Validates the schemas plugin has been registered
TEST(testMayaSchemas, verifyPlugin)
{
    PlugPluginPtr plug = PlugRegistry::GetInstance().GetPluginWithName("AL_USDMayaSchemas");
    EXPECT_TRUE(!plug.IsInvalid());
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/api.h" line="1">

---

# Maya API

This file defines the API for the Maya schemas used in the repository. It includes import and export definitions, which are used for integrating Maya with USD.

```c
//
// Copyright 2017 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef AL_USDMAYASCHEMAS_API_H
#define AL_USDMAYASCHEMAS_API_H

#include <pxr/base/arch/export.h>

#if defined(PXR_STATIC)
```

---

</SwmSnippet>

# Functions of Maya

This section discusses the main functions of Maya as implemented in the 'maya-usd' repository.

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/ModelAPI.h" line="35">

---

## AL_usd_ModelAPI

The `AL_usd_ModelAPI` class is used to import a Maya reference. It provides methods to set and get the selectability and lock state of a prim. It also provides a method to compute the selectability of a prim by looking at its own and its ancestor prims.

```c
PXR_NAMESPACE_OPEN_SCOPE

class SdfAssetPath;

// -------------------------------------------------------------------------- //
// ALMODELAPI                                                                 //
// -------------------------------------------------------------------------- //

/// \class AL_usd_ModelAPI
///
/// Data used to import a maya reference.
///
class AL_usd_ModelAPI : public UsdModelAPI
{
public:
    /// Compile time constant representing what kind of schema this class is.
    ///
    /// \sa UsdSchemaKind
    static const UsdSchemaKind schemaKind = UsdSchemaKind::NonAppliedAPI;

    /// Construct a AL_usd_ModelAPI on UsdPrim \p prim .
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/schema.usda.in" line="88">

---

## ALFrameRange

The `ALFrameRange` class represents the Maya Frame Range. It provides attributes to store the start and end animation frames, the current frame, and the min and max frames in Maya.

```in
class ALFrameRange "ALFrameRange"(
  doc = "Maya Frame Range"
  inherits = </Typed>
  customData = {
        string className = "FrameRange"
        string fileName = "FrameRange"
      }
)
{
    double animationStartFrame(
      doc = "The start animation frame in Maya."
    )
    double startFrame(
      doc = "The min frame in Maya."
    )
    double endFrame(
      doc = "The max frame in Maya."
    )
    double animationEndFrame(
      doc = "The end animation frame in Maya."
    )
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/tokens.cpp" line="1">

---

## AL_USDMayaSchemasTokens

The `AL_USDMayaSchemasTokens` class provides static, efficient tokens for use in all public USD API. These tokens represent property names and allowed tokens values declared for schema builtin attributes of 'token' scene description type.

```c++
//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "tokens.h"

PXR_NAMESPACE_OPEN_SCOPE

AL_USDMayaSchemasTokensType::AL_USDMayaSchemasTokensType()
    : animationEndFrame("animationEndFrame", TfToken::Immortal)
```

---

</SwmSnippet>

# AL_usd_FrameRange and AL_usd_ModelAPI Methods

Understanding AL_usd_FrameRange and AL_usd_ModelAPI classes

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/FrameRange.h" line="91">

---

## AL_usd_FrameRange::Get

The `AL_usd_FrameRange::Get` method is a static function that returns an `AL_usd_FrameRange` object associated with a given stage and path. If no such object exists, it returns an invalid schema object. This method is used to access the frame range data for a specific part of the USD scene.

```c
    AL_USDMAYASCHEMAS_API
    static AL_usd_FrameRange Get(const UsdStagePtr& stage, const SdfPath& path);
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/schemas/AL/usd/schemas/maya/ModelAPI.h" line="132">

---

## AL_usd_ModelAPI::GetSelectability

The `AL_usd_ModelAPI::GetSelectability` method returns the current selectability value of the prim. This method is used to check if a specific part of the USD scene is selectable in Maya.

```c
    TfToken GetSelectability() const;
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
