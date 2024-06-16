---
title: Understanding User Commands in Maya-USD
---

User Commands in the maya-usd repository refer to the functions that are executed in response to user actions. These commands are defined in various files within the repository, such as `baseExportCommand.cpp`, `baseImportCommand.cpp`, and `baseListShadingModesCommand.cpp`. The `doIt` function is a common example of a user command, which is used to execute a specific action. This function takes in arguments and performs actions based on these inputs. It is used across different modules for various purposes such as exporting and importing data, managing shading modes, and more. The commands are designed to interact with Maya's user interface and handle user inputs to provide desired outputs.

<SwmSnippet path="/lib/mayaUsd/commands/baseExportCommand.cpp" line="288">

---

## Usage of `doIt` method

The `doIt` method is a key part of User Commands. It is responsible for executing the command. In this example from the `baseExportCommand.cpp` file, the `doIt` method is used to export USD data. It takes an `MArgList` of arguments, parses them, and performs the export operation.

```c++
MStatus MayaUSDExportCommand::doIt(const MArgList& args)
{
    try {
        MStatus status;

        MArgDatabase argData(syntax(), args, &status);

        // Check that all flags were valid
        if (status != MS::kSuccess) {
            return status;
        }

        if (argData.isFlagSet("shadingMode")) {
            MString stringVal;
            argData.getFlagArgument("shadingMode", 0, stringVal);
            TfToken shadingMode(stringVal.asChar());

            if (!shadingMode.IsEmpty()
                && UsdMayaShadingModeRegistry::GetInstance().GetExporter(shadingMode) == nullptr
                && shadingMode != UsdMayaShadingModeTokens->none) {
                MGlobal::displayError(
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/primUpdaterManager.cpp" line="241">

---

## Invoking User Commands

User Commands are invoked by calling the `doIt` method. Here, in the `primUpdaterManager.cpp` file, the `doIt` method is invoked to perform an operation.

```c++
    return dgMod.doIt();
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/baseExportCommand.cpp" line="296">

---

## Error Handling in User Commands

Error handling is an important aspect of User Commands. If an error occurs during the execution of a command, the `doIt` method returns a failure status. This is demonstrated in this snippet from the `baseExportCommand.cpp` file.

```c++
        if (status != MS::kSuccess) {
            return status;
        }
```

---

</SwmSnippet>

# User Commands Overview

Understanding User Commands in MayaUSD

<SwmSnippet path="/lib/mayaUsd/commands/editTargetCommand.h" line="17">

---

## EditTargetCommand

The `EditTargetCommand` class is a command for editing the target of a USD stage. It inherits from `MPxCommand` which is a base class for creating Maya commands. The command provides functionalities such as creating the command, defining its syntax, and executing the command with `doIt`, `undoIt`, and `redoIt` methods.

```c
#ifndef MAYAUSD_COMMANDS_EDIT_TARGET_COMMAND_H
#define MAYAUSD_COMMANDS_EDIT_TARGET_COMMAND_H

#include <mayaUsd/base/api.h>
#include <mayaUsd/mayaUsd.h>

#include <maya/MPxCommand.h>
#include <maya/MString.h>

#include <memory>
#include <string>

namespace MAYAUSD_NS_DEF {

namespace Impl {
class SetEditTarget;
}

class EditTargetCommand : public MPxCommand
{
public:
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/layerEditorCommand.h" line="17">

---

## LayerEditorCommand

The `LayerEditorCommand` class is a command for editing USD layers. It also inherits from `MPxCommand`. This command provides functionalities for creating the command, defining its syntax, and executing the command. It also includes methods for handling queries and edits.

```c
#ifndef MAYAUSD_COMMANDS_LAYER_EDITOR_COMMAND_H
#define MAYAUSD_COMMANDS_LAYER_EDITOR_COMMAND_H

#include <mayaUsd/base/api.h>
#include <mayaUsd/mayaUsd.h>

#include <pxr/usd/sdf/layer.h>

#include <maya/MPxCommand.h>
#include <maya/MString.h>

#include <memory>
#include <string>
#include <vector>

namespace MAYAUSD_NS_DEF {

namespace Impl {
class BaseCmd;
}

```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
