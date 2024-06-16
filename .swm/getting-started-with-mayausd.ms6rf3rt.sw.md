---
title: Getting Started with MayaUsd
---

MayaUsd is a plugin for Autodesk Maya which provides a powerful bridge to the Pixar Universal Scene Description (USD) framework. It allows for efficient collaboration and interchange with the powerful USD framework. It includes features such as importing and exporting USD data, and editing the data directly within Maya. It also provides a set of utilities and commands for working with USD within Maya. The MayaUsd plugin is designed to be extensible and customizable, allowing for the integration of new features and workflows.

The MayaUsd plugin is organized into several directories, each serving a specific purpose. The 'lib' directory contains the core functionality of the plugin, including the main plugin registration and the USD scene interface. The 'ufe' directory contains the code for the Universal Front End (UFE) interface, which allows MayaUsd to interact with Maya's scene graph. The 'fileio' directory contains the code for reading and writing USD files. The 'nodes' directory contains the definitions for the various Maya nodes that the plugin provides. The 'commands' directory contains the implementation of various Maya commands that the plugin provides.

The MayaUsd plugin is built using CMake and can be built as a standalone plugin or as part of a larger project. It has dependencies on the USD library, as well as several Maya libraries. The build process generates a plugin file that can be loaded into Maya, as well as a Python module that provides a Python API to the plugin's functionality.

<SwmSnippet path="/lib/mayaUsd/ufe/MayaUsdContextOps.h" line="35">

---

# MayaUsdContextOps

The `MayaUsdContextOps` class provides the interface for Maya scene item context operations. It is used to provide contextual operation support, such as the Outliner context menu. It is not copy-able or move-able.

```c
class MAYAUSD_CORE_PUBLIC MayaUsdContextOps : public UsdUfe::UsdContextOps
{
public:
    typedef UsdUfe::UsdContextOps              Parent;
    typedef std::shared_ptr<MayaUsdContextOps> Ptr;

    MayaUsdContextOps(const UsdUfe::UsdSceneItem::Ptr& item);

    //! Create a MayaUsdContextOps.
    static MayaUsdContextOps::Ptr create(const UsdUfe::UsdSceneItem::Ptr& item);

    // UsdUfe::UsdContextOps overrides
    Items                     getItems(const ItemPath& itemPath) const override;
    Ufe::UndoableCommand::Ptr doOpCmd(const ItemPath& itemPath) override;

    Items                     getBulkItems(const ItemPath& itemPath) const override;
    Ufe::UndoableCommand::Ptr doBulkOpCmd(const ItemPath& itemPath) override;

    UsdUfe::UsdContextOps::SchemaNameMap getSchemaPluginNiceNames() const override;

}; // MayaUsdContextOps
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/resources/scripts/mayaUsdLibRegisterStrings.py" line="25">

---

# mayaUsdLibRegisterStrings

The `mayaUsdLibRegisterStrings` function is used to register strings that can be used in both MEL and Python. It is called from the equivalent MEL proc with the same name.

```python
def mayaUsdLibRegisterStrings():
    # This function is called from the equivalent MEL proc
    # with the same name. The strings registered here and all the
    # ones registered from the MEL proc can be used in either
    # MEL or python.

    # Any python strings from MayaUsd lib go here.

    # ae_template.py
    register('kKindMetadataAnn', 'Kind is a type of metadata (a pre-loaded string value) used to classify prims in USD. Set the classification value from the dropdown to assign a kind category to a prim. Set a kind value to activate selection by kind.')
    register('kActiveMetadataAnn', "If selected, the prim is set to active and contributes to the composition of a stage. If a prim is set to inactive, it doesn't contribute to the composition of a stage (it gets striked out in the Outliner and is deactivated from the Viewport).")
    register('kInstanceableMetadataAnn', 'If selected, instanceable is set to true for the prim and the prim is considered a candidate for instancing. If deselected, instanceable is set to false.')
    register('kErrorAttributeMustBeArray', '"^1s" must be an array!')
    register('kMenuCopyValue', 'Copy Attribute Value')
    register('kMenuPrintValue', 'Print to Script Editor')
    register('kLabelUnusedTransformAttrs', 'Unused')
    register('kLabelMetadata', 'Metadata')
    register('kLabelAppliedSchemas', 'Applied Schemas')
    register('kOpenImage', 'Open')
    register('kLabelMaterial', 'Material')
    register('kLabelAssignedMaterial', 'Assigned Material')
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/resources/scripts/mayaUsdUtils.py" line="224">

---

# showHelpMayaUSD

The `showHelpMayaUSD` function is a helper method to display help content. It is a wrapper around Maya's showHelp() method and should be used for all help contents in Maya USD.

```python
def showHelpMayaUSD(contentId):
    """
    Helper method to display help content.

    Note that this is a wrapper around Maya's showHelp() method and showHelpMayaUSD()
    should be used for all help contents in Maya USD.

    Example usage of this method:

    - In Python scripts:
    from mayaUsdUtils import showHelpMayaUSD
    showHelpMayaUSD("someContentId");

    - In MEL scripts:
    python(\"from mayaUsdUtils import showHelpMayaUSD; showHelpMayaUSD('someContentId');\")

    - In C++:
    MGlobal::executePythonCommand(
    "from mayaUsdUtils import showHelpMayaUSD; showHelpMayaUSD(\"someContentId\");");

    Input contentId refers to the contentId that is registered in helpTableMayaUSD
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/PullPushCommands.cpp" line="408">

---

# doIt

The `doIt` function is part of the MPxCommand API to execute the command. It is used to parse arguments, handle errors, and manage undo items.

```c++
// MPxCommand API to execute the command.
MStatus DuplicateCommand::doIt(const MArgList& argList)
{
    clearResult();

    setCommandString(commandName);

    PXR_NS::VtDictionary userArgs;

    MStatus    status = MS::kSuccess;
    MArgParser argParser(syntax(), argList, &status);
    if (status != MS::kSuccess)
        return status;

    status = parseUfePathArg(argParser, 0, _srcPath);
    if (status != MS::kSuccess)
        return reportError(status);

    status = parseUfePathArg(argParser, 1, _dstPath);
    if (status != MS::kSuccess)
        return reportError(status);
```

---

</SwmSnippet>

# MayaUsd Functions Overview

This section will cover the main functions of MayaUsd, focusing on the utilities provided for working with USD data within Maya.

## MayaUsd Utilities

The `lib/mayaUsd/utils` directory contains a variety of utility functions for working with USD data within Maya. These include functions for handling layers, color space conversions, and various other utilities for working with USD data. These functions are essential for the manipulation and management of USD data within the Maya environment.

<SwmSnippet path="/lib/mayaUsd/resources/scripts/mayaUsdUtils.py" line="224">

---

## showHelpMayaUSD Function

The `showHelpMayaUSD` function is a helper method used to display help content. It is a wrapper around Maya's `showHelp` method and should be used for all help contents in Maya USD. This function is essential for providing user guidance and assistance within the Maya USD plugin.

```python
def showHelpMayaUSD(contentId):
    """
    Helper method to display help content.

    Note that this is a wrapper around Maya's showHelp() method and showHelpMayaUSD()
    should be used for all help contents in Maya USD.

    Example usage of this method:

    - In Python scripts:
    from mayaUsdUtils import showHelpMayaUSD
    showHelpMayaUSD("someContentId");

    - In MEL scripts:
    python(\"from mayaUsdUtils import showHelpMayaUSD; showHelpMayaUSD('someContentId');\")

    - In C++:
    MGlobal::executePythonCommand(
    "from mayaUsdUtils import showHelpMayaUSD; showHelpMayaUSD(\"someContentId\");");

    Input contentId refers to the contentId that is registered in helpTableMayaUSD
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
