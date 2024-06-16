---
title: Understanding Maya Event Utilities
---

Maya Event Utilities in the maya-usd repository refers to a set of tools and functionalities that handle events within the Maya environment. These utilities are primarily used for managing and responding to various types of events triggered within Maya, such as animation, command, condition, container, DAG, DG, event, lock, model, node, object set, paint, poly, scene, timer, and UI messages. The utilities include classes and functions that register callbacks, bind message functions, and trigger events. They also provide structures to store binding information for the MMessage event info.

<SwmSnippet path="/plugin/al/mayautils/AL/maya/event/MayaEventManager.h" line="34">

---

## Maya Event Types

The `MayaMessageType` enum class defines the different types of Maya messages that can be handled by the event utilities. Each type corresponds to a different kind of event in the Maya environment.

```c
enum class MayaMessageType
{
    kAnimMessage,      ///< messages from the MAnimMessage class
    kCameraSetMessage, ///< messages from the MCameraSetMessage class
    kCommandMessage,   ///< messages from the MCommandMessage class
    kConditionMessage, ///< messages from the MConditionMessage class
    kContainerMessage, ///< messages from the MContainerMessage class
    kDagMessage,       ///< messages from the MDagMessage class
    kDGMessage,        ///< messages from the MDGMessage class
    kEventMessage,     ///< messages from the MEventMessage class
    kLockMessage,      ///< messages from the MLockMessage class
    kModelMessage,     ///< messages from the MModelMessage class
    kNodeMessage,      ///< messages from the MNodeMessage class
    kObjectSetMessage, ///< messages from the MObjectSetMessage class
    kPaintMessage,     ///< messages from the MPaintMessage class
    kPolyMessage,      ///< messages from the MPolyMessage class
    kSceneMessage,     ///< messages from the MSceneMessage class
    kTimerMessage,     ///< messages from the MTimerMessage class
    kUiMessage         ///< messages from the MUiMessage class
};
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/event/MayaEventManager.h" line="516">

---

## Maya Callback Information

The `MayaCallbackInfo` structure is used to store information about a callback for a specific event. This includes the event id, the callback function type, the Maya message type, and other relevant information.

```c
    /// \brief  a structure to store some binding info for the MMessage event info
    struct MayaCallbackInfo
    {
        AL::event::EventId eventId;         ///< the event id from the event scheduler
        uint32_t           refCount;        ///< the ref count for this callback
        MayaMessageType    mayaMessageType; ///< the maya MMessage class that defines the message
        MayaCallbackType
                 mayaCallbackType; ///< the type of C callback function needed to execute the callback
        uint32_t mmessageEnum; ///< the enum value from one of the MSceneMessage / MEventMessage etc
                               ///< classes.
        MCallbackId mayaCallback; ///< the maya callback ID
    };
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/event/MayaEventManager.h" line="546">

---

## Registering and Retrieving Event Information

The `getEventInfo` function is used to retrieve information about a specific event. It takes an event id as an argument and returns a pointer to the `MayaCallbackInfo` structure for that event.

```c
    /// \brief  queries the maya event information for the specified maya event
    /// \param  event the event ID
    /// \return a pointer to the maya event information (or null for an invalid event)
    const MayaCallbackInfo* getEventInfo(const AL::event::EventId event) const
    {
        const auto it = m_eventMapping.find(event);
        if (it == m_eventMapping.end())
            return 0;
        return m_callbacks.data() + it->second;
    }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/event/MayaEventManager.h" line="561">

---

## Checking if a Callback is Registered

The `isMayaCallbackRegistered` function is used to check if a callback is registered for a specific event. It takes an event id as an argument and returns a boolean value indicating whether a callback is registered for that event.

```c
    {
        const MayaCallbackInfo* cbi = getEventInfo(event);
        return cbi ? cbi->refCount != 0 : false;
```

---

</SwmSnippet>

# Maya Event Utilities Functions

This section covers the main functions of the Maya Event Utilities in the maya-usd repository.

<SwmSnippet path="/plugin/al/mayautils/AL/maya/event/MayaEventManager.h" line="655">

---

## MayaEventManager Class

The MayaEventManager class is responsible for managing Maya events. It provides methods to register and unregister callbacks for these events. The registerCallback method takes a function pointer, an event name, a unique tag, a weight, and an optional user data pointer, and returns a callback ID. The unregisterCallback method takes a callback ID and removes the corresponding callback.

```c
class MayaEventManager
{
    AL_MAYA_EVENTS_PUBLIC
    static MayaEventManager* g_instance;

public:
    /// \brief  returns the global maya event manager instance
    AL_MAYA_EVENTS_PUBLIC
    static MayaEventManager& instance();

    /// \brief  returns the global maya event manager instance
    static void freeInstance()
    {
        delete g_instance;
        g_instance = 0;
    }

    /// \brief  constructor
    /// \param  mayaEvents the custom event handler
    MayaEventManager(MayaEventHandler* mayaEvents)
        : m_mayaEvents(mayaEvents)
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/event/MayaEventManager.h" line="570">

---

## MayaEventHandler Class

The MayaEventHandler class is used by the MayaEventManager to handle the registration and execution of callbacks. It provides various registerEvent methods to register callbacks for specific types of Maya events, such as animation messages, camera set messages, command messages, and so on.

```c
    {
        const AL::event::EventDispatcher* const dispatcher = m_scheduler->event(eventName);
        if (dispatcher) {
            return getEventInfo(dispatcher->eventId());
        }
        return 0;
    }

    /// \brief  queries whether the event has an associated MCallbackId (indicating the callback is
    /// active with maya) \param  eventName the event to query \return true if callback is active
    /// with maya, false otherwise
    bool isMayaCallbackRegistered(const char* const eventName) const
    {
        const MayaCallbackInfo* cbi = getEventInfo(eventName);
        return cbi ? cbi->refCount != 0 : false;
    }

private:
    void onCallbackCreated(const AL::event::CallbackId callbackId) override;
    void onCallbackDestroyed(const AL::event::CallbackId callbackId) override;
    bool registerEvent(
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
