---
title: Introduction to the AL Plugin
---

The 'al Plugin' in the maya-usd repository refers to a set of functionalities provided by Animal Logic's plugin for Pixar's Universal Scene Description (USD). It is organized into various directories, each serving a specific purpose. For instance, the 'plugin/al/plugin' directory contains the main plugin code, while 'plugin/al/utils' contains utility functions. The 'plugin/al/schemas' directory contains schemas for the USD, and 'plugin/al/lib' houses the main library files. There are also directories for samples, tutorials, and documentation to assist developers in understanding and using the plugin.

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/PluginRegister.h" line="62">

---

## AL Namespace

The 'AL' namespace is used to encapsulate the code related to the 'al Plugin'. This helps to avoid naming conflicts with other parts of the codebase.

```c
namespace AL {
namespace usdmaya {
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/CommandGuiHelper.h" line="61">

---

## CommandGuiHelper Class

The 'CommandGuiHelper' class is an example of the functionality provided by the 'al Plugin'. It helps to auto-generate GUI code for creating menu items and option box dialogs in Maya. This class can be used to create custom commands and GUI elements for the user's specific needs.

```c
//----------------------------------------------------------------------------------------------------------------------
/// \brief  This class isn't really a wrapper around command options as such, it's mainly just a
/// helper
///         to auto generate some GUI code to create a menu item + option box dialog.
/// \note
/// The following example code demonstrates how to use the CommandGuiHelper class to autogenerate a
/// menu item, which will be available in the menu path  "USD" -> "polygons" -> "Create Cube"; This
/// will call the mel command "polyCube". The total command called will be something akin to:
///
///   polyCube -constructionHistory true -width 1 -height 1.1 -depth 1.2 -subdivisionsX 1
///   subdivisionsY 2 -subdivisionsZ 3 -name "pCube"
///
/// However all of the numeric values will actually be stored as optionVar's. (see the optionVar mel
/// command, or MGlobal class) If the command is "polyCube", and the flag is "constructionHistory",
/// then the optionVar used to store the preference will be "polyCube_constructionHistory".
///
/// Whilst I'm using "polyCube" as an example of how to use this class, you'd probably want to use
/// this for your own MPxCommand derived classes.
///
/// \code
/// {
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
