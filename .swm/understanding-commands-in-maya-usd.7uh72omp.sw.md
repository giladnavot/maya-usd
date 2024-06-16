---
title: Understanding Commands in Maya-USD
---
Commands in the maya-usd repository are classes derived from the MPxCommand class, which is a part of Maya's API. They are used to implement new commands in Maya using C++. These commands can be called from the Maya command line, and can be used in MEL or Python scripts. The commands in this repository are primarily used to manipulate and interact with USD data within Maya. For example, the 'doIt' method in the 'MayaUSDImportCommand' class is used to import USD data into Maya. Similarly, the 'doIt' method in the 'MayaUSDListShadingModesCommand' class is used to list the shading modes available for USD data. There are also commands for editing USD targets, duplicating USD data, and discarding edits to USD data.

<SwmSnippet path="/lib/mayaUsd/commands/baseImportCommand.cpp" line="154">

---

# Import Command

The `doIt` method in the `MayaUSDImportCommand` class is an example of a command. It takes a list of arguments, checks their validity, and performs an import operation based on these arguments. It also handles undo and redo operations.

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

The `doIt` method in the `MayaUSDListShadingModesCommand` class is another example of a command. It lists shading modes based on the provided arguments, and handles different flags such as 'export', 'import', and 'exportOptions'.

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

<SwmSnippet path="/lib/mayaUsd/commands/PullPushCommands.cpp" line="83">

---

# Command Syntax

The `createSyntaxWithUfeArgs` function is used to create the syntax for a command. It takes an integer parameter representing the number of string parameters representing UFE paths, and returns an `MSyntax` object.

```c++
// Create the syntax for a command taking some string parameters representing UFE paths.
MSyntax createSyntaxWithUfeArgs(int paramCount)
{
    MSyntax syntax;

    syntax.enableQuery(false);
    syntax.enableEdit(false);

    for (int i = 0; i < paramCount; ++i)
        syntax.addArg(MSyntax::kString);

    return syntax;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/layerEditorWindowCommand.cpp" line="106">

---

# Command Creation

The `creator` method is a plugin callback to create the command object. It returns a new instance of the command class, cast to `MPxCommand*`.

```c++
// plug-in callback to create the command object
void* LayerEditorWindowCommand::creator()
{
    return static_cast<MPxCommand*>(new LayerEditorWindowCommand());
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
