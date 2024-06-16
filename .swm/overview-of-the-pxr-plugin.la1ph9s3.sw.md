---
title: Overview of the pxr Plugin
---

The pxr Plugin in the maya-usd repository refers to a set of functionalities that are used to extend the capabilities of the base software. It is defined and used in various parts of the codebase. For instance, the `pxr_plugin` macro in `plugin/pxr/cmake/macros/Public.cmake` is used to define a new plugin. The `pxr_setup_plugins` function in the same file is used to set up these plugins. There are also several instances where the concept of a plugin is used, such as the `isPlugin` variable in the `_pxr_library` function in `plugin/pxr/cmake/macros/Private.cmake`. This variable is used to check if a certain library is a plugin. Furthermore, the `pxr_add_extra_plugins` function in `plugin/pxr/cmake/macros/Public.cmake` is used to add extra plugins to the build. Overall, the pxr Plugin is a crucial part of the maya-usd repository, enabling the extension of the base software's capabilities.

<SwmSnippet path="/plugin/pxr/cmake/macros/Public.cmake" line="413">

---

## Using the pxr_setup_plugins function

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

## Using the pxr_plugin macro

The `pxr_plugin` macro is used to create a library of the specified type. It calls the `pxr_library` function with the provided name and type.

```cmake
macro(pxr_plugin NAME)
    pxr_library(${NAME} TYPE "PLUGIN" ${ARGN})
endmacro(pxr_plugin)
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/cmake/macros/Private.cmake" line="974">

---

## Using the \_pxr_library function

The `_pxr_library` function is used to add a library target named NAME. It parses arguments, sets up the target, and computes names and paths.

```cmake
# Add a library target named NAME.
function(_pxr_library NAME)
    # Argument parsing.
    set(options
    )
    set(oneValueArgs
        PREFIX
        SUBDIR
        SUFFIX
        TYPE
        PRECOMPILED_HEADERS
        PRECOMPILED_HEADER_NAME
    )
    set(multiValueArgs
        PUBLIC_HEADERS
        PRIVATE_HEADERS
        CPPFILES
        LIBRARIES
        INCLUDE_DIRS
        RESOURCE_FILES
        LIB_INSTALL_PREFIX_RESULT
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/cmake/macros/Public.cmake" line="450">

---

## Using the pxr_add_extra_plugins function

The `pxr_add_extra_plugins` function is used to install a top-level plugInfo.json in the given plugin areas. It also updates the `PXR_EXTRA_PLUGINS` cache with the plugin paths.

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

# Export and Import Translators

Understanding Export and Import Translators

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/exportTranslator.h" line="48">

---

## UsdMayaExportTranslator::writer

The `UsdMayaExportTranslator::writer` function is an endpoint that handles the writing of USD data from Maya. It takes a file object, options string, and file access mode as arguments. This function is used when exporting USD data from Maya.

```c
        MPxFileTranslator::FileAccessMode mode) override;
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/importTranslator.h" line="44">

---

## UsdMayaImportTranslator::reader

The `UsdMayaImportTranslator::reader` function is an endpoint that handles the reading of USD data into Maya. It takes a file object, options string, and file access mode as arguments. This function is used when importing USD data into Maya.

```c
    MStatus reader(
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
