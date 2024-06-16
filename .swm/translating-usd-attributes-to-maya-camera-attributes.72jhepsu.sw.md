---
title: Translating USD Attributes to Maya Camera Attributes
---

This document will cover the process of translating USD attributes to Maya camera attributes, which includes:

1. Reading USD attributes to Maya camera
2. Translating USD attributes to Maya plug
3. Translating animated USD attributes to Maya plug
4. Getting time and value array for USD attribute
5. Checking USD type and resizing arrays.

## Where is this flow used?

The flow starts with the function `_ReadToCamera`. It is called from multiple entry points as represented in the following diagram:

```mermaid
graph TD;
subgraph lib/mayaUsd/fileio/translators/translatorCamera.cpp
  Read:::rootsStyle --> _ReadToCamera:::mainFlowStyle
end
subgraph lib/mayaUsd/fileio/translators/translatorCamera.cpp
  ReadToCamera:::rootsStyle --> _ReadToCamera:::mainFlowStyle
end

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

## The flow itself

```mermaid
graph TD;
subgraph lib/mayaUsd/fileio/translators/translatorCamera.cpp
  _ReadToCamera:::mainFlowStyle --> _TranslateUsdAttributeToPlug
end
subgraph lib/mayaUsd/fileio/translators/translatorCamera.cpp
  _TranslateUsdAttributeToPlug:::mainFlowStyle --> _TranslateAnimatedUsdAttributeToPlug
end
subgraph lib/mayaUsd/fileio/translators/translatorCamera.cpp
  _TranslateAnimatedUsdAttributeToPlug:::mainFlowStyle --> _GetTimeAndValueArrayForUsdAttribute
end
subgraph lib/mayaUsd/fileio/translators/translatorCamera.cpp
  _GetTimeAndValueArrayForUsdAttribute:::mainFlowStyle --> _CheckUsdTypeAndResizeArrays
end
  _CheckUsdTypeAndResizeArrays:::mainFlowStyle --> ...

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorCamera.cpp" line="1">

---

# Reading USD attributes to Maya camera

The function `_ReadToCamera` is the entry point for reading USD attributes and translating them to Maya camera attributes. It calls the `_TranslateUsdAttributeToPlug` function to start the translation process.

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
#include "translatorCamera.h"

#include <mayaUsd/fileio/jobs/jobArgs.h>
#include <mayaUsd/fileio/primReaderArgs.h>
#include <mayaUsd/fileio/primReaderContext.h>
#include <mayaUsd/fileio/shading/shadingModeRegistry.h>
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorCamera.cpp" line="272">

---

# Translating USD attributes to Maya plug

The function `_TranslateUsdAttributeToPlug` translates a single USD attribute to a Maya plug. It first checks for and translates animation if there is any. If that fails, it tries setting a static value.

```c++
static bool _TranslateUsdAttributeToPlug(
    const UsdAttribute&          usdAttr,
    const MFnCamera&             cameraFn,
    const TfToken&               plugName,
    const UsdMayaPrimReaderArgs& args,
    UsdMayaPrimReaderContext*    context,
    const MDistance::Unit        convertToUnit = MDistance::kMillimeters)
{
    MStatus status;

    MPlug plug = cameraFn.findPlug(plugName.GetText(), true, &status);
    CHECK_MSTATUS_AND_RETURN(status, false);

    // First check for and translate animation if there is any.
    if (!_TranslateAnimatedUsdAttributeToPlug(usdAttr, plug, args, context, convertToUnit)) {
        // If that fails, then try just setting a static value.
        UsdTimeCode timeCode = UsdTimeCode::EarliestTime();
        float       attrValue;
        usdAttr.Get(&attrValue, timeCode);

        switch (convertToUnit) {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorCamera.cpp" line="207">

---

# Translating animated USD attributes to Maya plug

The function `_TranslateAnimatedUsdAttributeToPlug` translates an animated USD attribute to a Maya plug. It calls the `_GetTimeAndValueArrayForUsdAttribute` function to get the time and value array for the USD attribute.

```c++
static bool _TranslateAnimatedUsdAttributeToPlug(
    const UsdAttribute&          usdAttr,
    MPlug&                       plug,
    const UsdMayaPrimReaderArgs& args,
    UsdMayaPrimReaderContext*    context,
    const MDistance::Unit        convertToUnit = MDistance::kMillimeters)
{
    if (args.GetTimeInterval().IsEmpty()) {
        return false;
    }

    MTimeArray   timeArray;
    MDoubleArray valueArray;
    if (!_GetTimeAndValueArrayForUsdAttribute(
            usdAttr,
            args.GetTimeInterval(),
            &timeArray,
            &valueArray,
            convertToUnit,
            (context != nullptr) ? context->GetTimeSampleMultiplier() : 1.0)) {
        return false;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorCamera.cpp" line="108">

---

# Getting time and value array for USD attribute

The function `_GetTimeAndValueArrayForUsdAttribute` gets the time and value array for a USD attribute. It calls the `_CheckUsdTypeAndResizeArrays` function to check the USD type and resize the arrays.

```c++
static bool _GetTimeAndValueArrayForUsdAttribute(
    const UsdAttribute&   usdAttr,
    const GfInterval&     timeInterval,
    MTimeArray*           timeArray,
    MDoubleArray*         valueArray,
    const MDistance::Unit convertToUnit = MDistance::kMillimeters,
    const double          timeSampleMultiplier = 1.0)
{
    static const TfType& floatType = TfType::Find<float>();
    std::vector<double>  timeSamples;

    if (!_CheckUsdTypeAndResizeArrays(
            usdAttr, floatType, timeInterval, &timeSamples, timeArray, valueArray)) {
        return false;
    }

    size_t      numTimeSamples = timeSamples.size();
    MTime::Unit timeUnit = MTime::uiUnit();
    for (size_t i = 0; i < numTimeSamples; ++i) {
        const double timeSample = timeSamples[i];
        float        attrValue;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorCamera.cpp" line="73">

---

# Checking USD type and resizing arrays

The function `_CheckUsdTypeAndResizeArrays` checks the type of the USD attribute and resizes the time and value arrays accordingly.

```c++
// clang-format on

static bool _CheckUsdTypeAndResizeArrays(
    const UsdAttribute&  usdAttr,
    const TfType&        expectedType,
    const GfInterval&    timeInterval,
    std::vector<double>* timeSamples,
    MTimeArray*          timeArray,
    MDoubleArray*        valueArray)
{
    // Validate that the attribute holds values of the expected type.
    const TfType type = usdAttr.GetTypeName().GetType();
    if (type != expectedType) {
        TF_CODING_ERROR(
            "Unsupported type name for USD attribute '%s': %s",
            usdAttr.GetName().GetText(),
            type.GetTypeName().c_str());
        return false;
    }

    if (!usdAttr.GetTimeSamplesInInterval(timeInterval, timeSamples)) {
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
