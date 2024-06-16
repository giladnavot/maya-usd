---
title: Introduction to File IO
---
File IO in the maya-usd repository refers to the operations related to reading from and writing to files. This is crucial for the functionality of the repository as it allows for the import and export of data between Maya and USD files. The repository contains several utilities and methods for handling file IO, such as the `FindOrCreateMayaAttr` and `_FindOrCreateMayaTypedAttr` methods in `readUtil.cpp`, which are used to read data from files. The `filename` method in `importData.cpp` and its usage across the repository is an example of writing to files. These operations are essential for the conversion and manipulation of data between Maya and USD formats.

<SwmSnippet path="/lib/mayaUsd/fileio/utils/readUtil.cpp" line="207">

---

# FindOrCreateMayaAttr Function

The `FindOrCreateMayaAttr` function is a key part of the File IO operations in this repository. It is used to find or create a Maya attribute based on the provided type, role, and variability. If the attribute does not exist, it will be created using the `_FindOrCreateMayaTypedAttr` function.

```c++
MObject UsdMayaReadUtil::FindOrCreateMayaAttr(
    const TfType&        type,
    const TfToken&       role,
    const SdfVariability variability,
    MFnDependencyNode&   depNode,
    const std::string&   attrName,
    const std::string&   attrNiceName,
    MDGModifier&         modifier)
{
    MString mayaName = attrName.c_str();
    MString niceName = attrNiceName.empty() ? attrName.c_str() : attrNiceName.c_str();

    // For the majority of things, we don't care about the role, just about
    // the type, e.g. we export point3f/vector3f/float3 the same.
    // (Though for stuff like colors, we'll disambiguate by role.)
    const bool keyable = variability == SdfVariabilityVarying;
    const bool usedAsColor = role == SdfValueRoleNames->Color;

    MObject attrObj;
    if (type.IsA<TfToken>()) {
        return _FindOrCreateMayaTypedAttr(
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/readUtil.cpp" line="98">

---

# \_FindOrCreateMayaTypedAttr Function

The `_FindOrCreateMayaTypedAttr` function is used by `FindOrCreateMayaAttr` to create a new Maya attribute of the specified type. It sets various properties of the attribute, such as whether it is keyable, used as a color, or used as a filename, based on the provided parameters.

```c++
static MObject _FindOrCreateMayaTypedAttr(
    const std::string&  attrName,
    const std::string&  attrNiceName,
    const MFnData::Type type,
    const bool          keyable,
    const bool          usedAsColor,
    const bool          usedAsFilename,
    MFnDependencyNode&  depNode,
    MDGModifier&        modifier)
{
    MString mayaName = attrName.c_str();
    MString niceName = attrNiceName.empty() ? attrName.c_str() : attrNiceName.c_str();

    MPlug plug = depNode.findPlug(mayaName, true);
    if (plug.isNull()) {
        // Create.
        MFnTypedAttribute attr;
        MObject           attrObj = attr.create(mayaName, mayaName, type);
        attr.setNiceNameOverride(niceName);
        attr.setKeyable(keyable);

```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
