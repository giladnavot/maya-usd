---
title: UV Set Handling Process
---

This document will cover the process of handling UV sets in the Maya-USD plugin, which includes:

1. Checking for new UV sets
2. Extracting Maya UV data
3. Guessing UV interpolation type
4. Ending the process.

```mermaid
graph TD;
subgraph plugin/al/usdmayautils/AL/usdmaya/utils
  hasNewUvSet:::mainFlowStyle --> extractMayaUvData
end
subgraph plugin/al/usdmayautils/AL/usdmaya/utils
  extractMayaUvData:::mainFlowStyle --> extractUvDataFromMaya
end
subgraph plugin/al/usdmayautils/AL/usdmaya/utils
  extractUvDataFromMaya:::mainFlowStyle --> guessUVInterpolationTypeExtensive
end
subgraph plugin/al/usdmayautils/AL/usdmaya/utils
  guessUVInterpolationTypeExtensive:::mainFlowStyle --> end
end
  end:::mainFlowStyle --> ...

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/DiffPrimVar.cpp" line="1">

---

# Checking for new UV sets

The function `hasNewUvSet` is the entry point for this flow. It checks if there are new UV sets in the Maya mesh that need to be processed.

```c++
//
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/DiffPrimVar.cpp" line="626">

---

# Extracting Maya UV data

The function `extractMayaUvData` is called next. It iterates over the existing set definitions and extracts UV data from the Maya mesh for each set.

```c++
//----------------------------------------------------------------------------------------------------------------------
void UvSetBuilder::extractMayaUvData(const MFnMesh& mesh)
{
    for (uint32_t i = 0, n = m_existingSetDefinitions.size(); i < n; ++i) {
        m_existingSetDefinitions[i].extractUvDataFromMaya(mesh, &m_existingSetNames[i]);
    }
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/DiffPrimVar.cpp" line="808">

---

# Guessing UV interpolation type

The function `guessUVInterpolationTypeExtensive` is then called. It performs an exhaustive test to determine the UV interpolation type based on the UV data extracted from the Maya mesh.

```c++
//----------------------------------------------------------------------------------------------------------------------
TfToken guessUVInterpolationTypeExtensive(
    MFloatArray&           u,
    MFloatArray&           v,
    MIntArray&             indices,
    MIntArray&             pointIndices,
    MIntArray&             faceCounts,
    std::vector<uint32_t>& indicesToExtract)
{
    // sanity check on input arrays
    if (indices.length() == 0 || pointIndices.length() == 0 || u.length() == 0 || v.length() == 0
        || faceCounts.length() == 0) {
        TF_RUNTIME_ERROR("Unable to process mesh UV's - Invalid array lengths provided");
        return UsdGeomTokens->faceVarying;
    }

    // if UV coords are all identical, we have a constant value
    if (UsdUfe::vec2AreAllTheSame(&u[0], &v[0], u.length())) {
        return UsdGeomTokens->constant;
    }

```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="322">

---

# Ending the process

The function `end` marks the end of this flow. It returns an iterator pointing to the end of the common transform attributes array.

```c
    inline std::array<MObject, transformAttributesCount>::const_iterator end() const
    {
        return m_commonTransformAttributes.cend();
    }
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
