---
title: Overview of the UsdUndoableCommand Class
---
This document will cover the following topics related to the `UsdUndoableCommand` class:

1. What is `UsdUndoableCommand`.
2. Variables and functions of `UsdUndoableCommand`.
3. An example of how to use `UsdUndoableCommand`.

```mermaid
graph TD;
 UsdUndoCapture --> UsdUndoableCommand:::currentBaseStyle
UsdUndoableCommand --> UsdUndoAddRefOrPayloadCommand
UsdUndoAddRefOrPayloadCommand --> UsdUndoAddPayloadCommand
UsdUndoAddRefOrPayloadCommand --> UsdUndoAddReferenceCommand
UsdUndoableCommand --> UsdUndoClearPayloadsCommand
UsdUndoableCommand --> UsdUndoToggleActiveCommand
UsdUndoableCommand --> 1[...]

 classDef currentBaseStyle color:#000000,fill:#7CB9F4
```

# What is UsdUndoableCommand

`UsdUndoableCommand` is a templated helper class that factors out common code for USD undoable commands. It is designed to handle the capture of USD data changes and undo and redo them. It is not meant to be used directly, but via the template class below. It simplifies debugging by having a central point for all USD commands.

<SwmSnippet path="/lib/usdUfe/ufe/UsdUndoableCommand.h" line="91">

---

# Variables and functions

`UsdUndoableCommand` is a class that inherits from `UsdUndoCapture` and `Cmd`. It has a constructor that allows passing arguments to the command base class. It overrides the `execute`, `undo`, and `redo` methods from `Ufe::UndoableCommand`.

```c
class UsdUndoableCommand
    : public UsdUndoCapture
    , public Cmd
{
public:
    // This constructor allows passing arguments to the command bae class.
    // The magic of templated function will elide this if not used.
    template <class... ARGS>
    UsdUndoableCommand(const ARGS&... values)
        : Cmd(values...)
    {
    }

    // Ufe::UndoableCommand overrides.
    // Implemented by the UsdUndoCapture base class.

    void execute() override { executeWithUndoCapture(); }
    void undo() override { undoUsdChanges(); }
    void redo() override { redoUsdChanges(); }
};
```

---

</SwmSnippet>

<SwmSnippet path="/lib/usdUfe/ufe/UsdUndoableCommand.h" line="99">

---

This is the constructor of `UsdUndoableCommand`. It allows passing arguments to the command base class.

```c
    UsdUndoableCommand(const ARGS&... values)
        : Cmd(values...)
    {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/usdUfe/ufe/UsdUndoableCommand.h" line="117">

---

# Usage example

`UsdFunctionUndoableCommand` is an example of how to use `UsdUndoableCommand`. It is a templated helper class for USD implementations of UFE commands where the implementation is in a function. This avoids having to write a whole class just to implement the single `executeImplementation()` virtual function.

```c
template <typename Cmd> class UsdFunctionUndoableCommand : public UsdUndoableCommand<Cmd>
{
public:
    // The function signature that implements the command.
    using Function = std::function<void()>;

    // This constructor allows passing arguments to the command bae class.
    // The magic of templated function will elide this if not used.
    template <class... ARGS>
    UsdFunctionUndoableCommand(const ARGS&... values, Function&& func)
        : UsdUndoableCommand<Cmd>(values...)
        , _func(func)
    {
    }

    // Implementation of UsdUndoCapture API.

    void executeImplementation() override { _func(); }

private:
    Function _func;
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="class"><sup>Powered by [Swimm](/)</sup></SwmMeta>
