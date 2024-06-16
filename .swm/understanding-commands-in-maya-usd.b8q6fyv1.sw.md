---
title: Understanding Commands in Maya-USD
---

Commands in the maya-usd repository are classes that inherit from the MPxCommand class, which is part of the Maya API. They encapsulate operations that can be performed in the Maya environment, such as importing and exporting USD files, listing shading modes, and manipulating the layer editor window. Commands are typically executed in response to user actions in the Maya interface, and they can be undoable, meaning that their effects can be reversed by the Maya undo system. The doIt method is a key part of a command, as it contains the code that is executed when the command is run.

<SwmSnippet path="/lib/mayaUsd/commands/baseImportCommand.cpp" line="154">

---

# Import Command

The `doIt` method in the `MayaUSDImportCommand` class is an example of a command. It performs an import operation, taking in arguments that specify the details of the import. It checks the validity of the flags, gets dictionary values, and sets the file name.

```c++
/* virtual */
MStatus MayaUSDImportCommand::doIt(const MArgList& args)
{
    // The import process has its own undo/redo recording.
    // See: UsdMaya_ReadJob::Undo() and Redo().
    OpUndoItemMuting undoInfoMuting;

    MStatus status;

    MArgDatabase argData(syntax(), args, &status);

    // Check that all flags were valid
    if (status != MS::kSuccess) {
        return status;
    }

    // Get dictionary values.
    const VtDictionary userArgs = UsdMayaUtil::GetDictionaryFromArgDatabase(
        argData, UsdMayaJobImportArgs::GetGuideDictionary());

    std::string mFileName;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/baseListShadingModesCommand.cpp" line="79">

---

# List Shading Modes Command

The `doIt` method in the `MayaUSDListShadingModesCommand` class is another example of a command. It lists the shading modes, checking if certain flags are set and appending the results accordingly.

```c++
MStatus MayaUSDListShadingModesCommand::doIt(const MArgList& args)
{
    MStatus      status;
    MArgDatabase argData(syntax(), args, &status);

    if (status != MS::kSuccess) {
        return status;
    }

    const bool useRegistryOnly = argData.isFlagSet("useRegistryOnly");

    if (argData.isFlagSet("export")) {
        // We have these default exporters which are always 1-2 in the options:
        appendToResult(UsdMayaShadingModeRegistry::GetMaterialConversionInfo(
                           UsdImagingTokens->UsdPreviewSurface)
                           .niceName.GetText());
        if (!useRegistryOnly) {
            appendToResult(_tokens->NoneNiceName.GetText());
            // Then we explore the registries:
            for (auto const& s : UsdMayaShadingModeRegistry::ListExporters()) {
                if (s != UsdMayaShadingModeTokens->useRegistry) {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/PullPushCommands.h" line="30">

---

# Pull Push Commands

The `PullPushBaseCommand` class and its derived classes like `EditAsMayaCommand`, `MergeToUsdCommand`, `DiscardEditsCommand`, and `DuplicateCommand` are examples of commands. They each have a `doIt` method that performs a specific operation.

```c
namespace MAYAUSD_NS_DEF {
namespace ufe {

//------------------------------------------------------------------------------
// PullPushBaseCommand
//------------------------------------------------------------------------------

//! \brief Base undoable command holding the undo item list.

class PullPushBaseCommand : public MPxCommand
{
public:
    //! \brief MPxCommand API to undo the command.
    MAYAUSD_CORE_PUBLIC
    MStatus undoIt() override;

    //! \brief MPxCommand API to redo the command.
    MAYAUSD_CORE_PUBLIC
    MStatus redoIt() override;

    //! \brief MPxCommand API to specify the command is undoable.
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/layerEditorWindowCommand.cpp" line="106">

---

# Layer Editor Window Command

The `creator` method in the `LayerEditorWindowCommand` class is used to create a new instance of the command. This is a common pattern for commands in the maya-usd repository.

```c++
// plug-in callback to create the command object
void* LayerEditorWindowCommand::creator()
{
    return static_cast<MPxCommand*>(new LayerEditorWindowCommand());
}
```

---

</SwmSnippet>

# Command Endpoints

Understanding Command Endpoints

<SwmSnippet path="/lib/mayaUsd/commands/editTargetCommand.h" line="17">

---

## EditTargetCommand

The `EditTargetCommand` class is a command endpoint that allows for the manipulation of the edit target of a USD stage. It inherits from the `MPxCommand` class, which is the base class for command plugins in Maya. The `doIt`, `undoIt`, and `redoIt` methods are overridden to define the behavior of the command when it is executed, undone, and redone respectively. The `creator` and `createSyntax` static methods are used by Maya to create instances of the command and to define its syntax.

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

<SwmSnippet path="/lib/mayaUsd/commands/layerEditorWindowCommand.h" line="17">

---

## LayerEditorWindowCommand

The `LayerEditorWindowCommand` class is another command endpoint that is used to manage the layer editor window in Maya. It also inherits from the `MPxCommand` class. This command is used to perform operations related to the layer editor window such as handling queries and edits. The `doIt`, `undoIt`, and `redoIt` methods are overridden to define the behavior of the command when it is executed, undone, and redone respectively.

```c
#ifndef LAYEREDITORWINDOWCOMMAND_H
#define LAYEREDITORWINDOWCOMMAND_H

#include <mayaUsd/base/api.h>

#include <maya/MArgParser.h>
#include <maya/MPxCommand.h>

namespace MAYAUSD_NS_DEF {

class AbstractLayerEditorWindow;

class MAYAUSD_CORE_PUBLIC LayerEditorWindowCommand : public MPxCommand
{
public:
    LayerEditorWindowCommand();
    // plugin registration requirements
    static const char commandName[];
    static void*      creator();
    static MSyntax    createSyntax();

```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
