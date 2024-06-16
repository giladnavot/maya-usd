---
title: Argument Parsing in Commands using MSyntax and MArgDatabase
---
This document will cover the process of argument parsing for commands with the use of 'MSyntax' and 'MArgDatabase' in the maya-usd repository. We will discuss:

1. How 'MSyntax' is used to define the syntax of a command.
2. How 'MArgDatabase' is used to parse the arguments of a command.
3. How these concepts are implemented in the maya-usd repository.

<SwmSnippet path="/lib/mayaUsd/commands/PullPushCommands.cpp" line="83">

---

# Defining Command Syntax with 'MSyntax'

The function 'createSyntaxWithUfeArgs' is used to define the syntax for a command that takes string parameters representing UFE paths. The 'MSyntax' object 'syntax' is used to specify the number and type of arguments the command accepts.

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

<SwmSnippet path="/lib/mayaUsd/commands/PullPushCommands.cpp" line="230">

---

The 'createSyntax' method of 'MergeToUsdCommand' class uses 'createSyntaxWithUfeArgs' to define the syntax for the command. It also adds additional flags to the syntax.

```c++
// MPxCommand API to register the command syntax.
MSyntax MergeToUsdCommand::createSyntax()
{
    MSyntax syntax = createSyntaxWithUfeArgs(1);
    syntax.addFlag(kExportOptionsFlag, kExportOptionsFlagLong, MSyntax::kString);
    syntax.addFlag(kIgnoreVariantsFlag, kIgnoreVariantsFlagLong, MSyntax::kBoolean);
    return syntax;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/PullPushCommands.cpp" line="100">

---

# Parsing Command Arguments with 'MArgDatabase'

The function 'parseTextArg' is used to parse the indexed argument as text. The 'MArgParser' object 'argParser' is used to retrieve the argument at the specified index.

```c++
// Parse the indexed argument as text.
MStatus parseTextArg(const MArgParser& argParser, int index, MString& outputText)
{
    argParser.getCommandArgument(index, outputText);
    if (outputText.length() <= 0)
        return MS::kNotFound;

    return MS::kSuccess;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/PullPushCommands.cpp" line="121">

---

The function 'parseDagPathArg' is used to parse the indexed argument as a DAG path. It uses 'parseTextArg' to retrieve the argument as text and then converts it to a DAG path.

```c++
MStatus parseDagPathArg(const MArgParser& argParser, int index, MDagPath& outputDagPath)
{
    MString text;
    MStatus status = parseTextArg(argParser, index, text);
    if (MS::kSuccess != status)
        return status;

    MObject obj;
    status = PXR_NS::UsdMayaUtil::GetMObjectByName(text, obj);
    if (status != MStatus::kSuccess)
        return status;

    return MDagPath::getAPathTo(obj, outputDagPath);
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/PullPushCommands.cpp" line="136">

---

The function 'parseTextArg' is also used to parse a flag argument. If the flag is set, the argument associated with the flag is retrieved.

```c++
MString parseTextArg(const MArgDatabase& argData, const char* flag, const MString& defaultValue)
{
    MString value = defaultValue;
    if (argData.isFlagSet(flag))
        argData.getFlagArgument(flag, 0, value);
    return value;
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="follow-up"><sup>Powered by [Swimm](/)</sup></SwmMeta>
