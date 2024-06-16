---
title: Registration and Creation of New Commands in Maya
---
This document will cover the process of registering and creating new commands in Maya from the maya-usd library, which includes:

1. Understanding the structure of the library
2. How commands are registered
3. How new commands are created.

# Understanding the structure of the library

The maya-usd library is structured into several directories, each serving a specific purpose. The `lib/mayaUsd/commands` directory contains the base commands that are used throughout the library. The `lib/mayaUsd/resources/scripts` directory contains scripts that are used to register and create new commands in Maya.

<SwmSnippet path="/lib/mayaUsd/resources/scripts/mayaUsdLibRegisterStrings.py" line="16">

---

# How commands are registered

Commands are registered using the `register` function. This function takes a key and a value, and registers them as a command in the Maya environment. The `getMayaUsdLibString` function is used to retrieve the registered command using the key.

```python
import maya.cmds as cmds
import maya.mel as mel

def register(key, value):
    registerPluginResource('mayaUsdLib', key, value)

def getMayaUsdLibString(key):
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/resources/scripts/mayaUsdAddMayaReference.py" line="21">

---

# How new commands are created

New commands are created by importing the `mayaUsdLibRegisterStrings` module and using the `getMayaUsdLibString` function. This function retrieves the registered command using the key, which can then be used to create a new command.

```python
import re
import maya.cmds as cmds
from mayaUsdLibRegisterStrings import getMayaUsdLibString
import mayaUsdMayaReferenceUtils as mayaRefUtils
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="follow-up"><sup>Powered by [Swimm](/)</sup></SwmMeta>
