---
title: Understanding Listeners in Maya-USD
---

Listeners in the maya-usd repository refer to the components that are designed to respond to specific events or changes in the USD stage. They are implemented as callback functions that get invoked when a particular event occurs. For instance, the UsdMayaStageNoticeListener class in the 'lib/mayaUsd/listeners/stageNoticeListener.h' file is a listener that can invoke callbacks in response to notices about a specific USD stage. It has methods to set callbacks for different types of stage notices such as StageContentsChanged, ObjectsChanged, LayerMutingChanged, and StageEditTargetChanged.

The listeners are designed to be flexible and efficient. They only register for notices if they are not already listening, and they stop listening for notices if either the stage or the callback is invalid. This is handled by the '\_UpdateStageContentsChangedRegistration' method in the 'lib/mayaUsd/listeners/stageNoticeListener.cpp' file.

Listeners also classify incoming changes that USD will consider either requiring an update (meaning the render delegate needs to refresh and redraw) or a resync (meaning the scene delegate needs to fetch new data). This is represented by the ChangeType enum class in the 'lib/mayaUsd/listeners/stageNoticeListener.h' file.

<SwmSnippet path="/lib/mayaUsd/listeners/stageNoticeListener.cpp" line="181">

---

# StageNoticeListener

This is an example of a listener in the maya-usd repository. The `_IsUiSchemaPrepend` function is a listener that checks if a UI schema is prepended.

```c++
namespace {
bool _IsUiSchemaPrepend(const VtValue& v)
{
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
