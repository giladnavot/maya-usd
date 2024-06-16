---
title: >-
  Exploring the library endpoints: initializePlugin and uninitializePlugin
  functions
---

This document will cover the role and usage of 'initializePlugin' and 'uninitializePlugin' functions in the maya-usd repository. We'll cover:

1. The purpose of 'initializePlugin' function
2. The purpose of 'uninitializePlugin' function
3. How these functions are used in the codebase.

<SwmSnippet path="/lib/mayaUsd/ufe/Global.cpp" line="184">

---

# The purpose of 'initializePlugin' function

The 'initialize' function is intended to be called by the plugin initialization to set up the stage model. It sets the Maya specific functions required for the UsdUfe plugin to work correctly and replaces the Maya hierarchy handler with ours. It also sets up a chain of responsibility for Transform3d interface creation.

```c++
MStatus initialize()
{
    // If we're already registered, do nothing.
    if (gRegistrationCount++ > 0)
        return MS::kSuccess;

    // Set the Maya specific functions required for the UsdUfe plugin to work correctly.
    UsdUfe::DCCFunctions dccFunctions;
    dccFunctions.stageAccessorFn = mayaGetStage;
    dccFunctions.stagePathAccessorFn = MayaUsd::ufe::stagePath;
    dccFunctions.ufePathToPrimFn = MayaUsd::ufe::ufePathToPrim;
    dccFunctions.timeAccessorFn = MayaUsd::ufe::getTime;
    dccFunctions.saveStageLoadRulesFn = MayaUsd::MayaUsdProxyShapeStageExtraData::saveLoadRules;
    dccFunctions.uniqueChildNameFn = MayaUsd::ufe::uniqueChildNameMayaStandard;
    dccFunctions.displayMessageFn[static_cast<int>(UsdUfe::MessageType::kInfo)]
        = displayInfoMessage;
    dccFunctions.displayMessageFn[static_cast<int>(UsdUfe::MessageType::kWarning)]
        = displayWarningMessage;
    dccFunctions.displayMessageFn[static_cast<int>(UsdUfe::MessageType::KError)]
        = displayErrorMessage;
    dccFunctions.startWaitCursorFn = mayaStartWaitCursor;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/plugin.cpp" line="95">

---

# The purpose of 'uninitializePlugin' function

The 'uninitializePlugin' function is used to deregister the plugin and clear any registered callbacks. It also handles the deregistration of commands and nodes.

```c++
PLUGIN_EXPORT MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin plugin(obj, "Autodesk", TOSTRING(MAYAUSD_VERSION), "Any");
    MStatus   ret = MS::kSuccess;
    if (auto* renderer = MHWRender::MRenderer::theRenderer()) {
        for (unsigned int i = 0; i < gsRenderOverrides.size(); i++) {
            renderer->deregisterOverride(gsRenderOverrides[i].get());
            gsRenderOverrides[i] = nullptr;
        }
    }

    // Note: when Maya is doing its default "quick exit" that does not uninitialize plugins,
    //       these overrides crash on destruction because Hydra ha already destroyed things
    //       these rely on. There is not much we can do about it...
    gsRenderOverrides.clear();

    // Clear any registered callbacks
    MGlobal::executeCommand("callbacks -cc mtoh;");

    if (!plugin.deregisterCommand(MtohViewCmd::name)) {
        ret = MS::kFailure;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/ufe/UsdScaleUndoableCommand.cpp" line="43">

---

# How 'initializePlugin' and 'uninitializePlugin' functions are used in the codebase

The 'initialize' function is used within the 'create' function in various files like 'UsdScaleUndoableCommand.cpp', 'UsdTranslateUndoableCommand.cpp', and 'UsdRotateUndoableCommand.cpp'. It is used to initialize the command after it's created.

```c++
    auto cmd = std::make_shared<MakeSharedEnabler<UsdScaleUndoableCommand>>(path, x, y, z);
    cmd->initialize();
    return cmd;
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/plugin/AL_USDMayaTestPlugin/plugin.cpp" line="36">

---

The 'uninitializePlugin' function is used in various plugin files like 'AL_USDMayaTestPlugin/plugin.cpp', 'AL_USDMayaPlugin/plugin.cpp', and 'pxrUsdPreviewSurface/plugin.cpp'. It is used to unregister the plugin during the uninitialization process.

```c++
MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin plugin(obj);
    AL_UNREGISTER_COMMAND(plugin, UnitTestHarness);
    return AL::usdmaya::unregisterPlugin(plugin);
}
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
