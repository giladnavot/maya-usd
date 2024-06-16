---
title: Overview of File Operations
---

File Operations in the maya-usd repository refer to the various operations that can be performed on files. This includes reading, writing, undoing, and redoing changes to files. These operations are crucial for the functioning of the Maya plugin and the reusable libraries for Pixar's Universal Scene Description (USD). The repository provides various methods and functions to perform these operations, such as the 'FindOrCreateMayaAttr' method for reading, the 'Read' method for reading files, the 'Redo' method for redoing changes, and the 'Undo' method for undoing changes. The 'filename' method is used to get the name of the file on which operations are being performed.

<SwmSnippet path="/lib/mayaUsd/fileio/jobs/readJob.cpp" line="194">

---

## Reading Data

The `Read` function is responsible for reading data from a USD file. It opens the file, reads the data, and updates the Maya scene accordingly. It also handles progress tracking and error checking.

```c++
bool UsdMaya_ReadJob::Read(std::vector<MDagPath>* addedDagPaths)
{
    // When we are called from PrimUpdaterManager we should already have
    // a computation scope. If we are called from elsewhere don't show any
    // progress bar here.
    MayaUsd::ProgressBarScope progressBar(16);

    // Do not use the global undo info recording system.
    // The read job Undo() / redo() functions will handle all operations.
    MayaUsd::OpUndoItemMuting undoMuting;

    MStatus status;

    if (!TF_VERIFY(!mImportData.empty())) {
        return false;
    }

    SdfLayerRefPtr rootLayer = SdfLayer::FindOrOpen(mImportData.filename());
    if (!rootLayer) {
        return false;
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/utils/readUtil.cpp" line="207">

---

## Creating and Managing Attributes

The `FindOrCreateMayaAttr` function is used to create or find a Maya attribute based on the data read from the USD file. It handles different types of data and sets the attribute properties accordingly.

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

<SwmSnippet path="/lib/mayaUsd/fileio/jobs/readJob.cpp" line="713">

---

## Undo and Redo Operations

The `Undo` function reverses changes to the Maya scene, while the `Redo` function re-applies them. These functions are essential for managing the state of the Maya scene and allowing users to reverse or reapply changes.

```c++
bool UsdMaya_ReadJob::Undo()
{
    // Do not use the global undo info recording system.
    // The read job Undo() / redo() functions will handle all operations.
    MayaUsd::OpUndoItemMuting undoMuting;

    // NOTE: (yliangsiew) All chasers need to have their Undo run as well.
    for (const UsdMayaImportChaserRefPtr& chaser : this->mImportChasers) {
        bool bStat = chaser->Undo();
        if (!bStat) {
            TF_WARN("Failed to execute import chaser's Redo()!");
            return false;
        }
    }

    if (!mDagModifierSeeded) {
        mDagModifierSeeded = true;
        MStatus dagStatus;
        // Construct list of top level DAG nodes to delete and any DG nodes
        for (auto& it : mNewNodeRegistry) {
            if (it.second != mMayaRootDagPath.node()) { // if not the parent root node
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
