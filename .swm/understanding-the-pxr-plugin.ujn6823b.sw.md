---
title: Understanding the pxr Plugin
---
The pxr Plugin in the maya-usd repository refers to a set of functionalities that are used to extend the capabilities of the base software. It is defined and used in various parts of the codebase. For instance, the `pxr_plugin` macro in `plugin/pxr/cmake/macros/Public.cmake` is used to define a new plugin. The `pxr_setup_plugins` function in the same file is used to install a top-level `plugInfo.json` in the shared area and into the top-level plugin area. The `pxr_add_extra_plugins` function is used to install a top-level `plugInfo.json` in the given plugin areas. The pxr Plugin is also used in the `plugin/pxr/maya/plugin/pxrUsd/plugin.cpp` file, where a constant called `_RegistrantId` is defined with the value 'pxrUsdPlugin'.

<SwmSnippet path="/plugin/pxr/cmake/macros/Public.cmake" line="413">

---

# pxr_setup_plugins function

The `pxr_setup_plugins` function is used to install a top-level plugInfo.json in the shared area and into the top-level plugin area. It also adds extra plugInfo.json include paths to the top-level plugInfo.json, relative to that top-level file.

```cmake
function(pxr_setup_plugins)
    # Install a top-level plugInfo.json in the shared area and into the
    # top-level plugin area
    _get_resources_dir_name(resourcesDir)

    # Add extra plugInfo.json include paths to the top-level plugInfo.json,
    # relative to that top-level file.
    set(extraIncludes "")
    list(REMOVE_DUPLICATES PXR_EXTRA_PLUGINS)
    foreach(dirName ${PXR_EXTRA_PLUGINS})
        file(RELATIVE_PATH
            relDirName
            "${CMAKE_INSTALL_PREFIX}/lib/usd"
            "${CMAKE_INSTALL_PREFIX}/${dirName}"
        )
        set(extraIncludes "${extraIncludes},\n        \"${relDirName}/\"")
    endforeach()

    set(plugInfoContents "{\n    \"Includes\": [\n        \"*/${resourcesDir}/\"${extraIncludes}\n    ]\n}\n")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/plugins_plugInfo.json"
         "${plugInfoContents}")
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/cmake/macros/Public.cmake" line="165">

---

# pxr_plugin macro

The `pxr_plugin` macro is used to create a library for a plugin. It calls the `pxr_library` function with the name of the plugin and the type set to 'PLUGIN'.

```cmake
macro(pxr_plugin NAME)
    pxr_library(${NAME} TYPE "PLUGIN" ${ARGN})
endmacro(pxr_plugin)
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/cmake/macros/Public.cmake" line="450">

---

# pxr_add_extra_plugins function

The `pxr_add_extra_plugins` function is used to install a top-level plugInfo.json in the given plugin areas. It also adds the plugin areas to the `PXR_EXTRA_PLUGINS` cache variable.

```cmake
function(pxr_add_extra_plugins PLUGIN_AREAS)
    # Install a top-level plugInfo.json in the given plugin areas.
    _get_resources_dir_name(resourcesDir)
    set(plugInfoContents "{\n    \"Includes\": [ \"*/${resourcesDir}/\" ]\n}\n")

    get_property(help CACHE PXR_EXTRA_PLUGINS PROPERTY HELPSTRING)

    foreach(area ${PLUGIN_AREAS})
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${area}_plugInfo.json"
            "${plugInfoContents}")
        install(
            FILES "${CMAKE_CURRENT_BINARY_DIR}/${area}_plugInfo.json"
            DESTINATION "${INSTALL_DIR_SUFFIX}/${PXR_INSTALL_SUBDIR}/${area}"
            RENAME "plugInfo.json"
        )
        list(APPEND PXR_EXTRA_PLUGINS "${PXR_INSTALL_SUBDIR}/${area}")
    endforeach()

    set(PXR_EXTRA_PLUGINS "${PXR_EXTRA_PLUGINS}" CACHE INTERNAL "${help}")
endfunction() # pxr_setup_third_plugins
```

---

</SwmSnippet>

# Function Explanations

Understanding the UsdMayaExportTranslator::writer and UsdMayaEditUtil::GetEditFromString functions

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/exportTranslator.h" line="44">

---

## UsdMayaExportTranslator::writer

The `UsdMayaExportTranslator::writer` function is a method in the `UsdMayaExportTranslator` class. It is responsible for writing out a USD file from a Maya scene. The function takes as input a `MFileObject` representing the file to write, a `MString` representing the options for the export, and a `FileAccessMode` indicating the mode of the file access (read, write, etc.).

```c
    PXRUSDMAYA_API
    MStatus writer(
        const MFileObject&                file,
        const MString&                    optionsString,
        MPxFileTranslator::FileAccessMode mode) override;
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/editUtil.h" line="94">

---

## UsdMayaEditUtil::GetEditFromString

The `UsdMayaEditUtil::GetEditFromString` function is a static method in the `UsdMayaEditUtil` class. It is used to translate an edit string into a `AssemblyEdit` structure. The function takes as input a `MFnAssembly` object representing the assembly to edit, a string representing the edit, and output parameters for the edit path and the edit itself.

```c
    static bool GetEditFromString(
        const MFnAssembly& assemblyFn,
        const std::string& editString,
        SdfPath*           outEditPath,
        AssemblyEdit*      outEdit);
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
