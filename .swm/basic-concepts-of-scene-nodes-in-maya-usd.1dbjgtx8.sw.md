---
title: Basic Concepts of Scene Nodes in Maya-USD
---

Scene Nodes in the maya-usd repository refer to the various nodes that are used to manage and manipulate the Universal Scene Description (USD) stages within Maya. These nodes provide the functionality to read, output, and share USD stages among multiple downstream consumer nodes. They also handle the specifics of reading and caching USD stages and layers.

One such Scene Node is the 'UsdMayaStageNode', which is a Maya dependency node that reads and outputs a USD stage. It reads in the USD stage identified by its file path attribute and makes that stage available as a stage data object on its output attribute. Downstream Maya nodes can connect this output to their own stage data input attributes to gain access to the stage.

Another important function related to Scene Nodes is 'isCopyingSceneNodes'. This function checks if Maya is currently copying nodes. When Maya copies nodes, it exports them and sets an environment variable during the export to let exporters know it is cutting or copying nodes in a temporary Maya scene file.

The 'LayerDatabase::setSelectedStage' function is another key part of Scene Nodes. It sets the selected stage in the Layer Database. If the selected stage is the same as the current one, it returns without making any changes. Otherwise, it sets the selected stage and marks the scene as modified.

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="192">

---

## Using `isCopyingSceneNodes`

The `isCopyingSceneNodes` function checks if the Scene Nodes are being copied. This is done by checking the environment variable `MAYA_CUT_COPY_EXPORT`. This function is used in various places in the codebase, such as in the `prepareForWriteCheck` and `saveUsd` functions.

```c++
bool isCopyingSceneNodes()
{
    // When Maya is copy nodes, it exports them and sets this environment
    // variable during the export to let exporters know it is cutting or
    // copying nodes in a temporary Maya scene file.
    return PXR_NS::ArchHasEnv("MAYA_CUT_COPY_EXPORT");
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="1109">

---

## Using `cleanUpNewScene`

The `cleanUpNewScene` function is used to clean up the Scene Nodes when a new scene is being set up. It does this by calling the `removeAllLayers` function, which removes all layers from the Scene Nodes.

```c++
void LayerDatabase::cleanUpNewScene(void*)
{
    LayerDatabase::instance().removeAllLayers();
    LayerDatabase::removeManagerNode();
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/nodes/layerManager.cpp" line="1188">

---

## Using `removeAllLayers`

The `removeAllLayers` function is used to remove all layers from the Scene Nodes. It is called by the `cleanUpNewScene` function when a new scene is being set up.

```c++
void LayerDatabase::removeAllLayers() { _idToLayer.clear(); }
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
