---
title: ProxyShape Processing Flow
---

This document will cover the process of initializing and processing ProxyShape in the Maya-USD plugin. The process includes:

1. Initializing the ProxyShapePostLoadProcess
2. Connecting Schema Prims
3. Creating Transform Chains for Schema Prims
4. Making USD Transform Chain
5. Constructing Lock Prims
6. Checking Prim Metadata
7. Updating Unique Key

```mermaid
graph TD;
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/cmds
  initialise:::mainFlowStyle --> connectSchemaPrims
end
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/cmds
  initialise:::mainFlowStyle --> createTranformChainsForSchemaPrims
end
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/fileio/translators
  connectSchemaPrims --> updateUniqueKey
end
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/fileio/translators
  updateUniqueKey --> getTranslatorFromId
end
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/cmds
  createTranformChainsForSchemaPrims:::mainFlowStyle --> doIt
end
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes
  createTranformChainsForSchemaPrims:::mainFlowStyle --> makeUsdTransformChain
end
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes
  makeUsdTransformChain:::mainFlowStyle --> doIt
end
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes
  doIt:::mainFlowStyle --> constructLockPrims
end
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes
  constructLockPrims:::mainFlowStyle --> isPrimLocked
end
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes
  isPrimLocked:::mainFlowStyle --> checkPrimMetadata
end
subgraph plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes
  checkPrimMetadata:::mainFlowStyle --> end
end
  end:::mainFlowStyle --> ...

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/cmds/ProxyShapePostLoadProcess.cpp" line="327">

---

# Initializing the ProxyShapePostLoadProcess

The function `ProxyShapePostLoadProcess::connectSchemaPrims` is the entry point for the process. It iterates over the prims we created, and calls any post-import logic to make any attribute connections etc.

```c++
//----------------------------------------------------------------------------------------------------------------------
void ProxyShapePostLoadProcess::connectSchemaPrims(
    nodes::ProxyShape*          proxy,
    const std::vector<UsdPrim>& objsToCreate)
{
    TF_DEBUG(ALUSDMAYA_TRANSLATORS).Msg("ProxyShapePostLoadProcess::connectSchemaPrims\n");

    fileio::translators::TranslatorContextPtr   context = proxy->context();
    fileio::translators::TranslatorManufacture& translatorManufacture
        = proxy->translatorManufacture();

    // iterate over the prims we created, and call any post-import logic to make any attribute
    // connections etc
    auto       it = objsToCreate.begin();
    const auto end = objsToCreate.end();
    for (; it != end; ++it) {
        UsdPrim                               prim = *it;
        fileio::translators::TranslatorRefPtr torBase = translatorManufacture.get(prim);
        if (torBase) {
            TF_DEBUG(ALUSDMAYA_TRANSLATORS)
                .Msg(
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/cmds/ProxyShapePostLoadProcess.cpp" line="327">

---

# Connecting Schema Prims

The function `ProxyShapePostLoadProcess::connectSchemaPrims` is responsible for connecting Schema Prims. It iterates over the prims we created, and calls any post-import logic to make any attribute connections etc.

```c++
//----------------------------------------------------------------------------------------------------------------------
void ProxyShapePostLoadProcess::connectSchemaPrims(
    nodes::ProxyShape*          proxy,
    const std::vector<UsdPrim>& objsToCreate)
{
    TF_DEBUG(ALUSDMAYA_TRANSLATORS).Msg("ProxyShapePostLoadProcess::connectSchemaPrims\n");

    fileio::translators::TranslatorContextPtr   context = proxy->context();
    fileio::translators::TranslatorManufacture& translatorManufacture
        = proxy->translatorManufacture();

    // iterate over the prims we created, and call any post-import logic to make any attribute
    // connections etc
    auto       it = objsToCreate.begin();
    const auto end = objsToCreate.end();
    for (; it != end; ++it) {
        UsdPrim                               prim = *it;
        fileio::translators::TranslatorRefPtr torBase = translatorManufacture.get(prim);
        if (torBase) {
            TF_DEBUG(ALUSDMAYA_TRANSLATORS)
                .Msg(
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/cmds/LayerCommands.cpp" line="204">

---

# Creating Transform Chains for Schema Prims

The function `LayerGetLayers::doIt` is responsible for creating Transform Chains for Schema Prims. It gets the shape node and the stage, and then depending on the flags set, it performs different operations.

```c++
//----------------------------------------------------------------------------------------------------------------------
MStatus LayerGetLayers::doIt(const MArgList& argList)
{
    TF_DEBUG(ALUSDMAYA_COMMANDS).Msg("LayerGetLayers::doIt\n");
    try {
        MStatus      status;
        MArgDatabase args(syntax(), argList, &status);
        if (!status)
            return status;
        AL_MAYA_COMMAND_HELP(args, g_helpText);

        nodes::ProxyShape* proxyShape = getShapeNode(args);
        UsdStageRefPtr     stage = proxyShape->usdStage();
        MStringArray       results;

        const bool useIdentifiers = args.isFlagSet("-id");
        if (args.isFlagSet("-rl")) {

            SdfLayerHandle rootLayer = stage->GetRootLayer();
            if (useIdentifiers) {
                setResult(AL::maya::utils::convert(rootLayer->GetIdentifier()));
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes/ProxyShapeSelection.cpp" line="855">

---

# Making USD Transform Chain

The function `SelectionUndoHelper::doIt` is responsible for making USD Transform Chain. It modifies the selection and constructs lock prims.

```c++
//----------------------------------------------------------------------------------------------------------------------
void SelectionUndoHelper::doIt()
{
    TF_DEBUG(ALUSDMAYA_SELECTION)
        .Msg(
            "ProxyShapeSelection::SelectionUndoHelper::doIt %lu %lu\n",
            m_insertedRefs.size(),
            m_removedRefs.size());
    m_proxy->m_pleaseIgnoreSelection = true;
    m_modifier1.doIt();
    m_modifier2.doIt();
    m_proxy->insertTransformRefs(m_insertedRefs, nodes::ProxyShape::kSelection);
    m_proxy->removeTransformRefs(m_removedRefs, nodes::ProxyShape::kSelection);
    m_proxy->selectedPaths() = m_paths;
    if (!m_internal) {
        MGlobal::setActiveSelectionList(m_newSelection, MGlobal::kReplaceList);
    }
    m_proxy->m_pleaseIgnoreSelection = false;
    m_proxy->constructLockPrims();
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes/proxy/ProxyShapeMetaData.cpp" line="134">

---

# Constructing Lock Prims

The function `ProxyShape::constructLockPrims` is responsible for constructing Lock Prims. It iterates over the required paths and checks if the prim is locked.

```c++
//----------------------------------------------------------------------------------------------------------------------
void ProxyShape::constructLockPrims()
{
    MProfilingScope profilerScope(
        _proxyShapeMetadataProfilerCategory, MProfiler::kColorE_L3, "Construct lock prims");

    TF_DEBUG(ALUSDMAYA_EVALUATION).Msg("ProxyShape::constructLockPrims\n");

    LockPrimCache cache;
    // iterate over the
    for (const auto& it : m_requiredPaths) {
        Scope* transformScope = it.second.getTransformNode();
        if (!transformScope) {
            continue;
        }

        const UsdPrim& prim = transformScope->transform()->prim();
        if (prim) {
            bool isLocked = isPrimLocked(prim, cache);

            MObject lockObject = transformScope->thisMObject();
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/nodes/ProxyShape.cpp" line="2107">

---

# Checking Prim Metadata

The function `checkPrimMetadata` is responsible for checking Prim Metadata. It checks if the prim has the required metadata.

```c++
bool checkPrimMetadata(
    const UsdPrim& prim,
    const TfToken& key,
    const TfToken& trueToken,
    const TfToken& falseToken,
    CacheT&        cache)
{
    TfHashSet<UsdPrim, boost::hash<UsdPrim>> cachedPrims;
    auto                                     updateCache = [&cache, &cachedPrims](bool value) {
        for (auto&& cachedPrim : cachedPrims) {
            cache.insert(std::make_pair(cachedPrim, value));
        }
        return value;
    };

    auto parent(prim);
    while (parent.IsValid() && !parent.IsPseudoRoot()) {
        auto it = cache.find(parent);
        if (it != cache.end()) {
            return it->second;
        }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/lib/AL_USDMaya/AL/usdmaya/fileio/translators/TranslatorContext.cpp" line="669">

---

# Updating Unique Key

The function `TranslatorContext::updateUniqueKey` is responsible for updating Unique Key. It generates a unique key for the prim and updates it.

```c++
//----------------------------------------------------------------------------------------------------------------------
void TranslatorContext::updateUniqueKey(const UsdPrim& prim)
{
    MProfilingScope profilerScope(
        _translatorContextProfilerCategory, MProfiler::kColorE_L3, "Update unique key");

    TF_DEBUG(ALUSDMAYA_TRANSLATORS).Msg("TranslatorContext::updateUniqueKey\n");

    const auto path(prim.GetPath());
    TF_DEBUG(ALUSDMAYA_TRANSLATORS)
        .Msg(
            "TranslatorContext::updateUniqueKey [generateUniqueKey] updating unique key for "
            "prim='%s'\n",
            path.GetText());

    std::string translatorId = getTranslatorIdForPath(path);
    auto translator = m_proxyShape->translatorManufacture().getTranslatorFromId(translatorId);
    if (translator) {
        auto it = find(path);
        if (it != m_primMapping.end() && it->path() == path) {
            auto key(translator->generateUniqueKey(prim));
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
