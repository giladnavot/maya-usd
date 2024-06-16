---
title: Getting Started with the AL Plugin
---
The 'al Plugin' in the maya-usd repository refers to a set of functionalities provided by Animal Logic's plugin. It is organized into various directories, each serving a specific purpose. For instance, the 'plugin/al/plugin' directory contains the main plugin code, while 'plugin/al/utils' houses utility functions. The 'plugin/al/schemas' directory contains schemas, which are specifications for how data should be structured in USD. The 'plugin/al/samples' directory provides sample data and examples for users to learn from. The 'plugin/al/mayautils' directory contains utilities for working with Maya, and 'plugin/al/lib' contains the main library code. The 'plugin/al/tutorials' directory provides tutorials for users, and 'plugin/al/docs' contains documentation for the plugin. The 'AL' namespace is used throughout the plugin code to encapsulate the plugin's functionalities.

# al Plugin Directory Structure

The 'al Plugin' directory contains various subdirectories such as 'AL_USDMayaTestPlugin' and 'AL_USDMayaPlugin'. These subdirectories contain the necessary files and resources required for the plugin to function.

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/PluginRegister.h" line="60">

---

# al Plugin Namespace

The 'al Plugin' uses the 'AL' namespace within its code. This namespace is used to encapsulate the code related to the plugin, preventing naming conflicts with other parts of the 'maya-usd' project or Maya itself.

```c
PXR_NAMESPACE_USING_DIRECTIVE

namespace AL {
namespace usdmaya {
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/CommandGuiHelper.h" line="61">

---

# CommandGuiHelper Class in al Plugin

The 'CommandGuiHelper' class is a part of the 'al Plugin'. This class is used to generate GUI code to create a menu item and option box dialog in Maya. It provides various methods to add different types of options to the GUI such as boolean, integer, and string options.

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

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
