---
title: 'Role of the doIt, undoIt, and redoIt Functions in a Command Class'
---
This document will cover the purpose and usage of the 'doIt', 'undoIt', and 'redoIt' functions in a command class within the Maya-USD plugin. We'll cover:

1. The purpose of 'doIt', 'undoIt', and 'redoIt' functions
2. How these functions are used in the codebase
3. The data flow involving these functions.

<SwmSnippet path="/lib/mayaUsd/undo/OpUndoItems.h" line="255">

---

# Purpose of 'doIt', 'undoIt', and 'redoIt' functions

The 'doIt', 'undoIt', and 'redoIt' functions are part of the FunctionUndoItem class. This class records data needed to undo or redo generic functions sub-operations. The 'doIt' function is used to execute a single sub-operation, while 'undoIt' and 'redoIt' are used to undo and redo a single sub-operation respectively.

```c
//------------------------------------------------------------------------------
// FunctionUndoItem
//------------------------------------------------------------------------------

/// \class FunctionUndoItem
/// \brief Record data needed to undo or redo generic functions sub-operations.
class FunctionUndoItem : public OpUndoItem
{
public:
    /// \brief create but do *not* execute functions and keep track of it.
    ///        Useful if the item execution has already been done.
    MAYAUSD_CORE_PUBLIC
    static void create(
        const std::string     name,
        std::function<bool()> redo,
        std::function<bool()> undo,
        OpUndoItemList&       undoInfo);

    /// \brief create but do *not* execute functions and keep track of it in the global undo list.
    ///        Useful if the item execution has already been done.
    MAYAUSD_CORE_PUBLIC
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/undo/OpUndoItems.cpp" line="522">

---

# Usage of 'doIt', 'undoIt', and 'redoIt' functions in the codebase

The 'redo' function in the UfeCommandUndoItem class is an example of how 'redoIt' is used in the codebase. It calls the 'redo' method of the command object, which is an instance of a class that implements the 'redoIt' function.

```c++
bool UfeCommandUndoItem::redo()
{
    _command->redo();
    return true;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorPrim.cpp" line="29">

---

# Data flow involving 'doIt', 'undoIt', and 'redoIt' functions

In the 'Read' function of the UsdMayaTranslatorPrim class, the 'execute' function of the UfeCommandUndoItem class is called. This function, in turn, calls the 'doIt' function of the command object. If the command needs to be undone or redone later, the 'undoIt' or 'redoIt' function of the command object would be called respectively.

```c++
void UsdMayaTranslatorPrim::Read(
    const UsdPrim&               prim,
    MObject                      mayaNode,
    const UsdMayaPrimReaderArgs& args,
    UsdMayaPrimReaderContext*    context)
{
    UsdGeomImageable primSchema(prim);
    MTime::Unit      timeUnit = MTime::uiUnit();
    double timeSampleMultiplier = (context != nullptr) ? context->GetTimeSampleMultiplier() : 1.0;
    if (!primSchema) {
        TF_CODING_ERROR("Prim %s is not UsdGeomImageable.", prim.GetPath().GetText());
        return;
    }

    // Gather visibility
    // If timeInterval is non-empty, pick the first available sample in the
    // timeInterval or default.
    UsdTimeCode         visTimeSample = UsdTimeCode::EarliestTime();
    std::vector<double> visTimeSamples;
    size_t              visNumTimeSamples = 0;
    if (!args.GetTimeInterval().IsEmpty()) {
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="follow-up"><sup>Powered by [Swimm](/)</sup></SwmMeta>
