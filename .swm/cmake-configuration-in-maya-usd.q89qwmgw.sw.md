---
title: CMake Configuration in Maya-USD
---
This document provides a detailed explanation of how CMake is used in the Maya-USD project. It will cover the configuration process, the options available, and how they are used in the build process.

# CMake Minimum Required Version

The minimum required version of CMake for this project is specified as 3.13 to 3.17. This means the project is compatible with any CMake version within this range.

# Project Name

The project name is defined as 'maya-usd'. This name is used by CMake to identify the project.

# Build Options

Several build options are defined using the `option` command. These options can be turned ON or OFF to control the build process. For example, `BUILD_MAYAUSD_LIBRARY` option is used to control whether to build Core USD libraries or not.

# Build-related Information

This section defines several variables that provide information about the build, such as the build number, Git commit hash, Git branch, and MayaUsd cut-id. These variables are used to report version information about the build.

# Global Options

This section sets several global options for the build. For example, `CMAKE_INSTALL_MESSAGE` is set to 'NEVER' to avoid noisy install messages. If the build is on Apple platform and `BUILD_UB2` is ON, it sets `CMAKE_OSX_ARCHITECTURES` to 'x86_64;arm64' and `CMAKE_OSX_DEPLOYMENT_TARGET` to 11.0.

# Modules and Definitions

This section includes several modules and sets definitions for the build. For example, `cmake/utils.cmake` is included which provides several utility functions for the build process. It also finds the required packages like Maya and Python, and sets the corresponding variables.

# Compiler Configuration

This line includes the `cmake/compiler_config.cmake` file which sets up the compiler configuration for the build.

# Gulrak Filesystem

This section includes the `cmake/gulrak.cmake` file if the MayaUSD library or the AL plugin is being built. The Gulrak filesystem library provides a filesystem API for C++.

# Google Test

If tests are being built, this section includes the `cmake/googletest.cmake` file, enables testing, and adds the test subdirectory. Google Test is a testing framework for C++.

# Libraries

This line adds the 'lib' subdirectory to the build. This directory contains the source code for the libraries being built.

# Plugins

This section adds the plugin subdirectories to the build if their corresponding build options are ON. These plugins include the Pixar USD plugin, the Animal Logic USD plugin, and the Autodesk USD plugin.

# Install

This section sets the `USD_INSTALL_LOCATION` variable. If `MAYAUSD_TO_USD_RELATIVE_PATH` is defined, it sets `USD_INSTALL_LOCATION` to the relative path from the install folder to the USD location. Otherwise, it sets `USD_INSTALL_LOCATION` to `PXR_USD_LOCATION`.

# Maya Module Files

This section configures and installs the Maya module files for the Autodesk plugin, the legacy Maya-To-Hydra plugin, the Pixar USD plugin, and the Animal Logic USD plugin. These module files are used by Maya to locate and load the plugins.

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="general-build-tool"><sup>Powered by [Swimm](/)</sup></SwmMeta>
