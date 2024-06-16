---
title: Introduction to USD Editing
---

USD Editing refers to the process of making changes to the Universal Scene Description (USD) data. This is facilitated by the use of undoable items and undo blocks. Undoable items represent individual operations that can be undone, such as transformations or attribute changes. Undo blocks, on the other hand, are used to group multiple edits into a single undo operation, allowing for more efficient manipulation of the USD data.

# USD Editing Components

This section describes how to use the `OpUndoItem` implementations to perform the actual work of USD Editing. For example, `MDagModifierUndoItem` is used for DAG operations.

<SwmSnippet path="/lib/mayaUsd/undo/MayaUsdUndoBlock.h" line="41">

---

# Collecting USD Edits

`MayaUsdUndoBlockCmd` is used to collect USD edits inside Maya MPxCommand. This is an important part of the USD Editing process as it allows for changes to be tracked and potentially undone if necessary.

```c
//! \brief MayaUsdUndoBlockCmd is used to collect USD edits inside Maya MPxCommand.
```

---

</SwmSnippet>

# USD Undo/Redo Support

The USD Editing process also includes support for undo/redo operations. This is facilitated by a mechanism that collects an inverse of each authoring operation for undo purposes.

# Undo Operations in USD Editing

Understanding Undo Operations in USD Editing

<SwmSnippet path="/lib/mayaUsd/undo/OpUndoItemValidator.h" line="24">

---

## OpUndoItemValidator

The `OpUndoItemValidator` class is used to validate the state of the global undo item list. It provides methods to validate if the list is empty or if a specific item is present in the list. It also keeps track of the number of validators in use.

```c
class OpUndoItem;

//! \brief Validate that the global undo item list is in the correct state.
//
// Only turned on in debug builds to help catch incorrect usages of OpUndoItem.
class MAYAUSD_CORE_PUBLIC OpUndoItemValidator
{
public:
    //! \brief Constructor validates the global item list.
    OpUndoItemValidator()
    {
        OpUndoItemValidator::increaseValidatorCount();
#ifdef WANT_VALIDATE_UNDO_ITEM
        OpUndoItemValidator::validateEmpty();
#endif
    }

    //! \brief Destructor.
    ~OpUndoItemValidator() { OpUndoItemValidator::decreaseValidatorCount(); }

    static bool validateEmpty();
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/undo/MayaUsdUndoBlock.h" line="29">

---

## MayaUsdUndoBlock

The `MayaUsdUndoBlock` class is used to collect multiple edits into a single undo operation. It is derived from the `UsdUfe::UsdUndoBlock` class. The `MayaUsdUndoBlockCmd` class is used to collect USD edits inside a Maya MPxCommand.

```c
//! \brief MayaUsdUndoBlock collects multiple edits into a single undo operation.
/*!
 */
class MAYAUSD_CORE_PUBLIC MayaUsdUndoBlock : public UsdUfe::UsdUndoBlock
{
public:
    MayaUsdUndoBlock();
    ~MayaUsdUndoBlock() override;

    MAYAUSD_DISALLOW_COPY_MOVE_AND_ASSIGNMENT(MayaUsdUndoBlock);
};

//! \brief MayaUsdUndoBlockCmd is used to collect USD edits inside Maya MPxCommand.
/*!
 */
class MAYAUSD_CORE_PUBLIC MayaUsdUndoBlockCmd : public MPxCommand
{
public:
    MayaUsdUndoBlockCmd(UsdUfe::UsdUndoableItem undoableItem);
    static void* creator();

```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
