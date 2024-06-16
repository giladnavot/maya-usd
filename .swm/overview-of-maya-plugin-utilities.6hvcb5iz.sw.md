---
title: Overview of Maya Plugin Utilities
---

Maya Plugin Utilities in the maya-usd repository refer to a set of utilities that assist in the development and management of plugins for the Maya 3D computer graphics application. These utilities provide functionalities such as ensuring a Maya plugin is loaded, registering plugin translator options, and providing a set of macros to simplify the creation and management of Maya nodes and commands. They are encapsulated within the 'mayautils' directory and are designed to streamline the process of integrating the Universal Scene Description (USD) framework with Maya through the maya-usd plugin.

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/Utils.cpp" line="37">

---

# ensureMayaPluginIsLoaded Function

The `ensureMayaPluginIsLoaded` function checks if a given plugin is loaded in Maya. If the plugin is not loaded, it attempts to load it using the `MGlobal::executeCommand` function. If the plugin is still not loaded after this, it returns false; otherwise, it returns true.

```c++
bool ensureMayaPluginIsLoaded(const MString& pluginName)
{
    if (MFnPlugin::findPlugin(pluginName) == MObject::kNullObj) {
        MGlobal::executeCommand(
            MString("catchQuiet( `loadPlugin -quiet \"") + pluginName + "\"`)", false, false);
        if (MFnPlugin::findPlugin(pluginName) == MObject::kNullObj) {
            return false;
        }
    }
    return true;
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/PluginTranslatorOptions.cpp" line="105">

---

# PluginTranslatorOptions Class

The `PluginTranslatorOptions` class is used to manage a plugin's export and import options. It is initialized with a `PluginTranslatorOptionsContext` and a plugin translator grouping. The constructor registers the `PluginTranslatorOptions` instance with the provided context.

```c++
PluginTranslatorOptions::PluginTranslatorOptions(
    PluginTranslatorOptionsContext& context,
    const char* const               pluginTranslatorGrouping)
    : m_grouping(pluginTranslatorGrouping)
    , m_options()
    , m_context(context)
{
    context.registerPluginTranslatorOptions(this);
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/MayaHelperMacros.h" line="23">

---

# Maya Helper Macros

The Maya Helper Macros are a set of pre-defined macros that handle common tasks such as error checking and node registration. These macros simplify the code and make it more readable by abstracting away the underlying implementation details.

```c
#define AL_MAYA_CHECK_ERROR(status, ErrorString)                   \
    {                                                              \
        MStatus _status_##__LINE__ = status;                       \
        if (!_status_##__LINE__) {                                 \
            MString maya_error_string = __FILE__ ":";              \
            maya_error_string += __LINE__;                         \
            maya_error_string += " ";                              \
            maya_error_string += _status_##__LINE__.errorString(); \
            maya_error_string += " : ";                            \
            maya_error_string += ErrorString;                      \
            MGlobal::displayError(maya_error_string);              \
            return status;                                         \
        }                                                          \
    }

/// \brief  Given the status, validates that the status is ok. If not, an error is logged using the
/// specified error
///         message. If an error occurs, the program execution continues.
/// \ingroup   mayautils
#define AL_MAYA_CHECK_ERROR2(status, ErrorString)                  \
    {                                                              \
```

---

</SwmSnippet>

# Maya Plugin Utilities Functions

This section provides an overview of the main functions in the Maya Plugin Utilities.

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/PluginTranslatorOptions.cpp" line="31">

---

## registerPluginTranslatorOptions

The 'registerPluginTranslatorOptions' function is used to register plugin translator options. It takes an instance of 'PluginTranslatorOptions' as an argument and adds it to the 'm_optionGroups' vector. It also sets the 'm_dirty' flag to true, indicating that the options have been modified.

```c++
//----------------------------------------------------------------------------------------------------------------------
AL_MAYA_UTILS_PUBLIC
void PluginTranslatorOptionsContext::registerPluginTranslatorOptions(
    PluginTranslatorOptions* options)
{
    m_optionGroups.push_back(options);
    m_dirty = true;
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/PluginTranslatorOptions.cpp" line="84">

---

## unregisterPluginTranslatorOptions

The 'unregisterPluginTranslatorOptions' function is used to unregister plugin translator options. It takes a character string representing the plugin translator grouping as an argument. It finds the group in the 'm_optionGroups' vector and removes it if found.

```c++
//----------------------------------------------------------------------------------------------------------------------
AL_MAYA_UTILS_PUBLIC
void PluginTranslatorOptionsContext::unregisterPluginTranslatorOptions(
    const char* const pluginTranslatorGrouping)
{
    auto group = find(m_optionGroups, pluginTranslatorGrouping);
    if (group != m_optionGroups.end()) {
        m_optionGroups.erase(group);
    }
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/PluginTranslatorOptions.cpp" line="95">

---

## isRegistered

The 'isRegistered' function checks if a plugin translator grouping is registered. It returns true if the grouping is found in the 'm_optionGroups' vector, and false otherwise.

```c++
//----------------------------------------------------------------------------------------------------------------------
AL_MAYA_UTILS_PUBLIC
bool PluginTranslatorOptionsContext::isRegistered(const char* const pluginTranslatorGrouping) const
{
    auto group = find(m_optionGroups, pluginTranslatorGrouping);
    return group != m_optionGroups.end();
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/PluginTranslatorOptions.cpp" line="40">

---

## find

The 'find' function is a utility function used to find a specific group in the 'PluginTranslatorOptionsArray'. It takes the array and a character string representing the group name as arguments. It iterates over the array and returns an iterator to the group if found, or an iterator to the end of the array if not found.

```c++
//----------------------------------------------------------------------------------------------------------------------
PluginTranslatorOptionsArray::iterator
find(PluginTranslatorOptionsArray& array, const char* const groupName)
{
    for (auto it = array.begin(); it != array.end(); ++it) {
        if ((*it)->grouping() == groupName)
            return it;
    }
    return array.end();
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/Utils.cpp" line="36">

---

## ensureMayaPluginIsLoaded

The 'ensureMayaPluginIsLoaded' function checks if a Maya plugin is loaded, and attempts to load it if it's not. It takes the plugin name as an argument and returns true if the plugin is loaded, and false otherwise.

```c++
//----------------------------------------------------------------------------------------------------------------------
bool ensureMayaPluginIsLoaded(const MString& pluginName)
{
    if (MFnPlugin::findPlugin(pluginName) == MObject::kNullObj) {
        MGlobal::executeCommand(
            MString("catchQuiet( `loadPlugin -quiet \"") + pluginName + "\"`)", false, false);
        if (MFnPlugin::findPlugin(pluginName) == MObject::kNullObj) {
            return false;
        }
    }
    return true;
}
```

---

</SwmSnippet>

# Maya Plugin Utilities

Understanding Maya Plugin Utilities

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/FileTranslatorOptions.cpp" line="67">

---

## OptionsParser::parse

The `OptionsParser::parse` method is used to parse a string containing a semi-colon separated list of options passed to a file translator plugin. It returns a status indicating whether the parsing was successful or not.

```c++
MStatus OptionsParser::parse(const MString& optionString)
{
    MStatus status = MS::kSuccess;
    {
        auto it = m_niceNameToValue.begin();
        auto end = m_niceNameToValue.end();
        for (; it != end; ++it) {
            it->second->init();
        }
    }

    if (optionString.length() > 0) {
        int i, length;
        // Start parsing.
        MStringArray optionList;
        MStringArray theOption;
        optionString.split(';', optionList); // break out all the options.

        length = optionList.length();
        for (i = 0; i < length; ++i) {
            theOption.clear();
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/FileTranslatorOptions.cpp" line="169">

---

## FileTranslatorOptions::addBool

The `FileTranslatorOptions::addBool` method is used to add a boolean option to the file translator options. It takes the name of the option and its default value as parameters.

```c++
bool FileTranslatorOptions::addBool(const char* niceName, bool defaultValue)
{
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
