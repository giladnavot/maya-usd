---
title: Configuring and Installing Icons with CMake in lib/mayaUsd/resources/icons
---
This document provides a detailed walkthrough of how CMake is used in the `lib/mayaUsd/resources/icons` directory of the maya-usd project.

# Outliner Icons Configuration

This section sets up the Outliner icons. The `set` command is used to define a list of base icon names. The `foreach` loop then iterates over each base icon name, installing the corresponding PNG files to the `lib/icons` directory in the installation prefix. The base icon name is used to construct the file names of the PNG files. Note that the `_100.png` files are installed without the `_100` suffix, as Maya will automatically choose the `_150` or `_200` image if needed.

# Library Icons Configuration

This section sets up the Library icons in a similar manner to the Outliner icons. A list of base icon names is defined using the `set` command, and the `foreach` loop installs the corresponding PNG files to the `lib/icons` directory in the installation prefix. Again, the `_100.png` files are installed without the `_100` suffix, as Maya will automatically choose the `_150` or `_200` image if needed.

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="build-tool"><sup>Powered by [Swimm](/)</sup></SwmMeta>
