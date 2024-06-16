---
title: Overview of Utilities in Maya-USD
---

Utilities in maya-usd refer to a set of helper functions and classes that assist in various tasks throughout the codebase. They are organized in different directories such as 'plugin/al/utils', 'plugin/al/usdmayautils', and 'plugin/al/mayautils'. These utilities provide reusable code snippets that help in maintaining the code's efficiency and readability.

<SwmSnippet path="/plugin/al/utils/AL/event/EventHandler.h" line="512">

---

## Using Event Handlers

Event handlers are a type of utility used to manage events within the plugin. They can be used to handle specific events and perform certain actions when those events occur.

```c
    ///         handler (so the event will become invalidated afterwards). Please do not use this
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/MeshUtils.h" line="127">

---

## Using Mesh Utilities

Mesh utilities are used to manipulate and manage mesh data within the plugin. They provide functions to perform operations on meshes, such as setting data using the Maya function set.

```c
    MFnMesh            fnMesh;     ///< the maya function set to use when setting the data
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/NodeHelper.h" line="89">

---

## Using Node Helpers

Node helpers are utilities that provide functions to work with Maya nodes. They can be used to perform operations on nodes, such as setting and getting attribute values.

```c
///         In order to use this class, you should inherit from which ever MPxNode type you need
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
