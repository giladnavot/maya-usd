---
title: Exploring the Event Handler
---
An Event Handler in the maya-usd repository refers to a system that manages and dispatches internal events. It is implemented through the `CustomEventHandler` class, which provides an interface for the event system with utilities from the underlying DCC application. This class allows for the creation and destruction of custom event handlers. The `EventSystemBinding` class provides the event system with some utilities from the underlying DCC application, including logging and executing Python and MEL code. The `NodeEvents` class defines an interface that can be applied to custom nodes to allow them the ability to manage and dispatch internal events.

<SwmSnippet path="/plugin/al/utils/AL/event/EventHandler.h" line="213">

---

## CustomEventHandler Class

The `CustomEventHandler` class is an interface that provides the event system with some utilities from the underlying DCC application. It has methods to handle the creation and destruction of callbacks, and to return the event type as a string.

```c
class CustomEventHandler
{
public:
    /// \brief  dtor
    virtual ~CustomEventHandler() { }

    /// \brief  returns the event type as a string
    /// \return the eventType as a text string
    virtual const char* eventTypeString() const = 0;

    /// \brief  override if you need to insert custom event handler
    /// \param  callbackId the ID of the callback that has been created
    virtual void onCallbackCreated(const CallbackId callbackId) { }

    /// \brief  override if you need to remove a custom event handler
    /// \param  callbackId the ID of the callback that has been destroyed
    virtual void onCallbackDestroyed(const CallbackId callbackId) { }
};
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/utils/AL/event/EventHandler.h" line="1055">

---

## NodeEvents Class

The `NodeEvents` class defines an interface that can be applied to custom nodes to allow them the ability to manage and dispatch internal events. It provides methods to register, unregister, and trigger events, among others.

```c
class NodeEvents
{
    typedef std::unordered_map<std::string, EventId> EventMap;

public:
    /// \brief  ctor
    /// \param  scheduler the event scheduler
    AL_EVENT_PUBLIC
    NodeEvents(EventScheduler* const scheduler = &EventScheduler::getScheduler())
        : m_scheduler(scheduler)
    {
    }

    /// \brief  trigger the event of the given name
    /// \param  eventName the name of the event to trigger on this node
    /// \return true if the events triggered correctly
    bool triggerEvent(const char* const eventName)
    {
        auto it = m_events.find(eventName);
        if (it != m_events.end()) {
            return m_scheduler->triggerEvent(
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/utils/AL/event/EventHandler.h" line="124">

---

## EventSystemBinding Class

The `EventSystemBinding` class is an interface that provides the event system with some utilities from the underlying DCC application. It provides methods to execute Python and MEL code, write logs, and return the event type as a string and the total number of event types supported by the underlying system.

```c
class EventSystemBinding
{
public:
    /// \brief  ctor
    /// \param  eventTypeStrings the event types as text strings
    /// \param  numberOfEventTypes the number of event types in the event strings
    EventSystemBinding(const char* const eventTypeStrings[], size_t numberOfEventTypes)
        : m_eventTypeStrings(eventTypeStrings)
        , m_numberOfEventTypes(numberOfEventTypes)
    {
    }

    /// \brief  the logging severity
    enum Type
    {
        kInfo,
        kWarning,
        kError
    };

    /// \brief  log an info message
```

---

</SwmSnippet>

# Event Handler Functions

The Event Handler system in the 'maya-usd' repository provides a way to manage and dispatch internal events. This system includes several functions that are used to log information, trigger events, and register new events.

<SwmSnippet path="/plugin/al/utils/AL/event/EventHandler.h" line="144">

---

## Logging Functions

The logging functions 'info', 'error', and 'warning' are used to log messages with different severity levels. These functions take a text string as a parameter and log this string using the 'writeLog' function. The severity level of the message is determined by the function that is used.

```c
    /// \brief  log an info message
    /// \param  text printf style text string
    void info(const char* text, ...)
    {
        char    buffer[1024];
        va_list args;
        va_start(args, text);
        vsnprintf(buffer, 1024, text, args);
        writeLog(kInfo, buffer);
        va_end(args);
    }

    /// \brief  log an error message
    /// \param  text printf style text string
    void error(const char* text, ...)
    {
        char    buffer[1024];
        va_list args;
        va_start(args, text);
        vsnprintf(buffer, 1024, text, args);
        writeLog(kError, buffer);
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/utils/AL/event/EventHandler.h" line="605">

---

## triggerEvent Function

The 'triggerEvent' function is used to dispatch an event to all registered callbacks. This function takes no parameters and triggers the event by calling the appropriate callback function for each registered callback.

```c
    /// \brief  a default version of dispatchEvent that assumes a function callback type of
    /// \code
    /// void (*function_prototype)(void* userData);
    /// \endcode
    void triggerEvent()
    {
        for (auto& callback : m_callbacks) {
            if (callback.isCCallback()) {
                defaultEventFunction basic = (defaultEventFunction)callback.callback();
                basic(callback.userData());
            } else if (callback.isPythonCallback()) {
                if (!m_system->executePython(callback.callbackText())) {
                    m_system->error(
                        "The python callback of event name \"%s\" and tag \"%s\" failed to execute "
                        "correctly",
                        m_name.c_str(),
                        callback.tag().c_str());
                }
            } else {
                if (!m_system->executeMEL(callback.callbackText())) {
                    m_system->error(
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/utils/AL/event/EventHandler.h" line="1111">

---

## registerEvent Function

The 'registerEvent' function is used to register a new event. This function takes several parameters including the name of the event, the type of the event, and optional parameters for associated data and a parent callback. The function returns a boolean value indicating whether the event was successfully registered.

```c
    /// \brief  registers an event on this node
    /// \param  eventName the name of the event to register
    /// \param  eventType the type of event to register
    /// \param  parentId the callback id of the callback that triggers this event (or null)
    /// \return true if the event could be registered
    bool registerEvent(
        const char* const eventName,
        const EventType   eventType,
        const CallbackId  parentId = 0)
    {
        EventId id = m_scheduler->registerEvent(eventName, eventType, this, parentId);
        if (id) {
            m_events.emplace(eventName, id);
        }
        return id != InvalidEventId;
    }
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
