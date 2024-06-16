---
title: Understanding File IO
---
File IO in the maya-usd repository refers to the operations related to reading from and writing to files. This is crucial for the functioning of the plugin as it needs to read data from USD files and write data to Maya files.&nbsp;

The repository contains several utilities and methods to facilitate these operations. For instance, the `FindOrCreateMayaAttr` function in `readUtil.cpp` is used to find or create a Maya attribute based on the given parameters. Similarly, the `filename` method in `importData.cpp` is used to get the filename associated with the import data. These operations are spread across multiple files and classes in the repository, making File IO a fundamental part of the codebase.

<SwmSnippet path="/lib/mayaUsd/fileio/utils/readUtil.cpp" line="207">

---

# FindOrCreateMayaAttr Function

The `FindOrCreateMayaAttr` function is a key part of the File IO operations in this codebase. It is used to find or create a Maya attribute based on the provided parameters. This function is versatile and can handle a variety of data types, as seen in the different conditional branches within the function.

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

The `_FindOrCreateMayaTypedAttr` function is a helper function used by `FindOrCreateMayaAttr`. It is responsible for creating a new Maya attribute of a specific type if it does not already exist. This function also sets various properties of the attribute, such as whether it is keyable, used as a color, or used as a filename.

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

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](https://staging.swimm.cloud/)</sup></SwmMeta>
