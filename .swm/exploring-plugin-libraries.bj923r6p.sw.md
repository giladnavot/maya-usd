---
title: Exploring Plugin Libraries
---

Plugin Libraries in the maya-usd repository refer to the reusable components that are used to extend the functionality of the Maya software. These libraries are used to create a Maya plugin for Pixar's Universal Scene Description (USD). They include functions and classes that help in reading, writing, and translating data between Maya and USD. They also provide utilities for handling USD scenes, managing render tasks, and interacting with the Maya viewport. The plugin libraries are organized in a modular structure, allowing developers to use only the components they need, reducing dependencies and improving code manageability.

<SwmSnippet path="/lib/mayaUsd/fileio/utils/adaptor.cpp" line="172">

---

# Plugin Libraries Usage

In this code snippet, the `GetAppliedSchemas` function is defined. This function uses plugin adaptors to adapt schemas for import and export. It checks if the adaptor is valid, and if it is, it iterates over the adaptor entries to adapt the schema. If the schema can be adapted, it is added to the result.

```c++
TfTokenVector UsdMayaAdaptor::GetAppliedSchemas() const
{
    if (!*this) {
        return TfTokenVector();
    }

    if (_jobImportArgs && !_handle.isValid()) {
        TF_CODING_ERROR("An importing adaptor should only use ApplySchema.");
        return TfTokenVector();
    }

    TfTokenVector result;

    // See if we have any plugin adaptors we could use:
    const MFnDependencyNode depNodeFn(_handle.object());
    const std::string       mayaTypeName(depNodeFn.typeName().asChar());
    for (auto&& adaptorEntry : UsdMayaSchemaApiAdaptorRegistry::Find(mayaTypeName)) {
        TfToken schemaName(adaptorEntry.first);
        if (_jobExportArgs
            && _jobExportArgs->includeAPINames.find(schemaName)
                == _jobExportArgs->includeAPINames.end()) {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/pxrUsdMayaGL/sceneDelegate.cpp" line="538">

---

# Plugin Libraries in Render Tasks

In this code snippet, the `GetRenderTasks` function is defined. This function uses plugin libraries to create render tasks for different shapes and collections. It checks if the plugin is supported and if it is, it creates a render delegate and adds it to the task list.

```c++
HdTaskSharedPtrVector PxrMayaHdSceneDelegate::GetRenderTasks(
    const size_t                     hash,
    const PxrMayaHdRenderParams&     renderParams,
    unsigned int                     displayStyle,
    const PxrMayaHdPrimFilterVector& primFilters)
{
    HdTaskSharedPtrVector taskList;
    HdRenderIndex&        renderIndex = GetRenderIndex();

    // Task List Consist of:
    //  Render Setup Task
    //  Render Task Per Shape Adapter/Collection
    //  Selection Task
    taskList.reserve(2 + primFilters.size());

    SdfPath renderSetupTaskId;
    if (!TfMapLookup(_renderSetupTaskIdMap, hash, &renderSetupTaskId)) {
        // Create a new render setup task if one does not exist for this hash.
        renderSetupTaskId = _rootId.AppendChild(
            TfToken(TfStringPrintf("%s_%zx", HdxPrimitiveTokens->renderSetupTask.GetText(), hash)));

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/primWriterRegistry.cpp" line="206">

---

# Plugin Libraries in Prim Writers

In this code snippet, the `CheckForWriterPlugin` function is defined. This function uses plugin libraries to check for a writer plugin for a given Maya type name. If the type name is not in the registry, it loads the plugin.

```c++
/* static */
void UsdMayaPrimWriterRegistry::CheckForWriterPlugin(const std::string& mayaTypeName)
{
    TfRegistryManager::GetInstance().SubscribeTo<UsdMayaPrimWriterRegistry>();

    _Registry::const_iterator first, last;
    std::tie(first, last) = _reg.equal_range(mayaTypeName);

    if (first == last) {
        // If the type name is not currently in our registry, check for plugin registry
        static const TfTokenVector SCOPE = { _tokens->UsdMaya, _tokens->PrimWriter };
        UsdMaya_RegistryHelper::FindAndLoadMayaPlug(SCOPE, mayaTypeName);
    }
}
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
