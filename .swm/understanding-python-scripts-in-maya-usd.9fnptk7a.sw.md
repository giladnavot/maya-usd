---
title: Understanding Python Scripts in Maya-USD
---

Python scripts in the maya-usd repository are used to extend and customize the functionality of the Maya plugin. They are primarily used for creating, updating, and unregistering Python classes. These scripts are also responsible for handling errors and exceptions, ensuring the smooth operation of the plugin. The Python scripts are organized in a way that allows for efficient tracking and management of registered Python classes.

<SwmSnippet path="/lib/mayaUsd/python/pythonObjectRegistry.cpp" line="72">

---

# ClassName Method

The `ClassName` method is used to extract the name of a Python class. It first checks if the object is a Python class, and if it is, it extracts and returns the name of the class.

```c++
std::string UsdMayaPythonObjectRegistry::ClassName(object cl)
{
    // Is it a Python class:
    if (!IsPythonClass(cl)) {
        // So far class is always first parameter, so we can have this check be here.
        TfPyThrowRuntimeError("First argument must be a Python class");
    }

    auto nameAttr = cl.attr("__name__");
    if (!nameAttr) {
        TfPyThrowRuntimeError("Unexpected Python error: No __name__ attribute");
    }

    return std::string(boost::python::extract<std::string>(nameAttr));
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/python/pythonObjectRegistry.cpp" line="49">

---

# UnregisterPythonObject Method

The `UnregisterPythonObject` method is used to deregister a Python object. It finds the object in the index and if found, it clears the Python class.

```c++
void UsdMayaPythonObjectRegistry::UnregisterPythonObject(object cl, const std::string& key)
{
    TClassIndex::const_iterator target = _sIndex.find(key);
    if (target != _sIndex.cend()) {
        // Clear the Python class:
        _sClassVec[target->second] = object();
    }
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/python/pythonObjectRegistry.cpp" line="100">

---

# OnInterpreterExit Method

The `OnInterpreterExit` method is used to release all Python classes when the interpreter exits. It iterates over all the Python classes and sets them to an empty object.

```c++
void UsdMayaPythonObjectRegistry::OnInterpreterExit()
{
    // Release all Python classes:
    for (size_t i = 0; i < _sClassVec.size(); ++i) {
        _sClassVec[i] = object();
    }
}
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
