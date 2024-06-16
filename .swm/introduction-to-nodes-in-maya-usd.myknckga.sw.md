---
title: Introduction to Nodes in Maya-USD
---

Nodes in the maya-usd repository refer to the building blocks of a scene in Maya. They are objects that hold data and perform computations. They can represent geometric objects, materials, textures, lights, cameras, and more. Nodes are interconnected to form a dependency graph, which Maya uses to compute output data from input data. For example, the `UsdMayaStageNode` is a node that reads and outputs a USD stage, allowing multiple downstream consumer nodes to share a single USD stage.

<SwmSnippet path="/lib/mayaUsd/nodes/hdImagingShape.cpp" line="182">

---

# Node Creation

This code snippet shows the `GetOrCreateInstance` method in the `PxrMayaHdImagingShape` class. This method is used to create or retrieve an instance of a node. It first checks if the node already exists by looking up its UUID. If it doesn't exist, it creates a new node and sets its properties.

```c++
/* static */
MObject PxrMayaHdImagingShape::GetOrCreateInstance()
{
    MStatus status;

    // Look up the imaging shape via UUID; this is namespace-independent.
    MSelectionList selList;
    selList.add(_HdImagingShapeUuid);

    MObject hdImagingShapeObj;
    if (!selList.isEmpty() && selList.getDependNode(0, hdImagingShapeObj)) {
        return hdImagingShapeObj;
    }

    // Ensure that we create the nodes in the root namespace, in case this
    // function is getting invoked by a node in a non-root namespace (e.g. a USD
    // proxy shape that represents the "Collapsed" representation of an
    // assembly).
    const MString currNamespace = MNamespace::currentNamespace(&status);
    CHECK_MSTATUS_AND_RETURN(status, MObject());
    const MString rootNamespace = MNamespace::rootNamespace(&status);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/undo/OpUndoItems.cpp" line="493">

---

# Node Addition

This code snippet shows the `add` method in the `UfeCommandUndoItem` class. This method is used to add a node to the undo stack. The undo stack is a list of nodes that have been modified, allowing the user to undo their actions.

```c++
bool UfeCommandUndoItem::add(
    const std::string&                           name,
    const std::shared_ptr<Ufe::UndoableCommand>& command,
    OpUndoItemList&                              undoInfo)
{
    auto item = std::make_unique<UfeCommandUndoItem>(name, command);
    undoInfo.addItem(std::move(item));
    return true;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/undo/OpUndoItemList.h" line="114">

---

# Node Verification

This code snippet shows the `isEmpty` method in the `OpUndoItemList` class. This method is used to check if the undo stack is empty. This is useful to determine if there are any actions that can be undone.

```c
    /// \brief verify if the list is empty.
    bool isEmpty() const { return _undoItems.empty(); }
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
