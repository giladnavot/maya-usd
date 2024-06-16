---
title: Exploring USD Operations
---

USD Operations in the maya-usd repository refer to the actions performed on Pixar's Universal Scene Description (USD) data. These operations include creating, undoing, and redoing changes to the USD data. The operations are encapsulated in the `UsdUndoableItemUndoItem` class, which is a type of `OpUndoItem`. This class records the data needed to undo or redo USD sub-operations. It provides methods to create a USD undo item recorder, undo a single sub-operation, and redo a single sub-operation. The `create` method is used to instantiate a `UsdUndoableItemUndoItem`, which is then used in various parts of the codebase to perform operations on USD data.

# USD Undo/Redo Support

The primary motivation for this service is to restore USD data model changes to its correct state after undo/redo calls. The current implementation for this system uses [SdfLayerStateDelegateBase](https://graphics.pixar.com/usd/docs/api/class_sdf_layer_state_delegate_base.html#details) by implementing a mechanism to collect an inverse of each authoring operation for undo purposes. This mechanism was inspired by Luma's USD undo/redo facilities found in [usdQt](https://github.com/LumaPictures/usd-qt).

# Using the Framework

The general pattern to use the framework is as follow: Within the large operation, use the `OpUndoItem` implementation already provided to do the actual work. For example, use `MDagModifierUndoItem` to do DAG operations.

<SwmSnippet path="/lib/mayaUsd/undo/OpUndoItems.h" line="178">

---

# Recording USD Sub-Operations

The `OpUndoItems` class is used to record data needed to undo or redo USD sub-operations. It also provides a method to create a USD undo item recorder and keep track of it.

```c
/// \brief Record data needed to undo or redo USD sub-operations.
class UsdUndoableItemUndoItem : public OpUndoItem
{
public:
    /// \brief create a USD undo item recorder and keep track of it.
```

---

</SwmSnippet>

# USD Undo/Redo Support System

USD Undo/Redo Support System

## UsdUndoManager

The UsdUndoManager is responsible for temporarily collecting inverse edits for every state change made to a SdfLayer. It later transfers these collected edits into an UsdUndoableItem. It operates as a global singleton instance and needs to be informed which SdfLayer to track for changes. When a layer is set to be tracked, a UsdUndoStateDelegate is created and set unto it, which is how edits are detected.

## UsdUndoStateDelegate

The UsdUndoStateDelegate is set on a SdfLayer and is invoked on every authoring operation on that layer. This delegate is created by UsdUndoManager::trackLayerStates() when called by StagesSubject::stageEditTargetChanged() and StagesSubject::onStageSet(). It creates a function to do the inverse of each edit and adds those inverting functions to the UsdUndoManager.

## UsdUndoBlock

The UsdUndoBlock is meant to be instantiated on the stack and is thus active for its lifetime. During that lifetime, it collects all edits into a single undo operation. Nested UsdUndoBlock instances declared on the stack in different nested functions work much like SdfChangeBlock. That means only the top level UsdUndoBlock will transfer the edits to an UsdUndoableItem.

## MayaUsdUndoBlock

The MayaUsdUndoBlock class derives from UsdUndoBlock. It creates its own UsdUndoableItem internally when needed. Once the last MayaUsdUndoBlock is about to be destroyed, it creates a Maya command to hold onto the UsdUndoableItem permanently and thus connect the item undo and redo functions to the Maya undo and redo system.

## UsdUndoableItem

The UsdUndoableItem is the object that ultimately stores all the edits. It has an undo function to undo all the recorded edits and a redo function to redo them afterward. This is the object that is meant to be kept long-term to do the undo and redo.

# Undo Operations in USD

Understanding Undo Operations in USD

<SwmSnippet path="/lib/mayaUsd/undo/MayaUsdUndoBlock.h" line="29">

---

## MayaUsdUndoBlock

The `MayaUsdUndoBlock` class is used to collect multiple edits into a single undo operation. This is useful when you want to group several changes together so that they can be undone or redone as a single operation.

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
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/undo/OpUndoItemList.h" line="35">

---

## OpUndoItem

The `OpUndoItem` class represents a single undo sub-operation. It provides methods to execute, undo, and redo the operation. Each `OpUndoItem` has a name, which can be used for debugging and logging purposes.

```c
/// \class OpUndoItem
/// \brief Record data needed to undo or redo a single undo sub-operation.
///
/// See OpUndoItems.h for concrete implementations.

class OpUndoItem
{
public:
    typedef std::unique_ptr<OpUndoItem> Ptr;

    /// \brief construct a single named sub-operation.
    MAYAUSD_CORE_PUBLIC
    OpUndoItem(std::string name)
        : _name(std::move(name))
    {
    }

    MAYAUSD_CORE_PUBLIC
    virtual ~OpUndoItem() = default;

    /// \brief execute a single sub-operation. By default calls redo.
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
