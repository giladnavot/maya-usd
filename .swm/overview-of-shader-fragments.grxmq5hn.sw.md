---
title: Overview of Shader Fragments
---
Shader Fragments in the maya-usd repository refer to reusable pieces of shader code that are used to create a complete shader program. They are defined in the `vp2ShaderFragments` directory and are used to handle different aspects of the rendering process, such as lighting, color management, and geometry processing. The `HdVP2ShaderFragments` class provides methods to register and deregister these fragments. Each fragment is associated with a specific rendering stage and has a unique name. The fragments are loaded from XML files during the registration process.

<SwmSnippet path="/lib/mayaUsd/render/vp2ShaderFragments/shaderFragments.cpp" line="267">

---

## Shader Fragments Registration

The `registerFragments` function is used to register all the shader fragments with the MFragmentManager. It checks if the fragments are already registered, and if not, it adds them from their respective XML files. The function also handles the registration of language-specific fragments and fragment graphs.

```c++
MStatus HdVP2ShaderFragments::registerFragments()
{
    // If we're already registered, do nothing.
    if (_registrationCount > 0) {
        _registrationCount++;
        return MS::kSuccess;
    }

    MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
    if (!theRenderer) {
        return MS::kFailure;
    }

    MHWRender::MFragmentManager* fragmentManager = theRenderer->getFragmentManager();
    if (!fragmentManager) {
        return MS::kFailure;
    }

    std::string language;

    switch (theRenderer->drawAPI()) {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2ShaderFragments/shaderFragments.cpp" line="575">

---

## Shader Fragments Deregistration

The `deregisterFragments` function is used to deregister all the shader fragments from the MFragmentManager. It checks if the fragments are still in use by other plugins, and if not, it removes them. The function also handles the deregistration of domain shader fragment input name mappings and automatic shader stage input parameters.

```c++
MStatus HdVP2ShaderFragments::deregisterFragments()
{
    // If it was never registered, leave as-is:
    if (_registrationCount == 0) {
        return MS::kSuccess;
    }

    // If more than one plugin still has us registered, do nothing.
    if (_registrationCount > 1) {
        _registrationCount--;
        return MS::kSuccess;
    }
    MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
    if (!theRenderer) {
        return MS::kFailure;
    }

    MHWRender::MFragmentManager* fragmentManager = theRenderer->getFragmentManager();
    if (!fragmentManager) {
        return MS::kFailure;
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2ShaderFragments/shaderFragments.cpp" line="191">

---

## Shader Fragments Usage

The `AutomaticShaderStageInput` struct is used to define automatic shader stage input parameters, which are registered with the MFragmentManager and used in the creation of shader instances. The struct includes the shader stage, parameter name, parameter semantic, parameter type, and a flag indicating whether the input is varying.

```c++
//! Structure for Automatic shader stage input parameter to register in VP2.
struct AutomaticShaderStageInput
{
    MHWRender::MFragmentManager::ShaderStage  _shaderStage;
    MString                                   _parameterName;
    MString                                   _parameterSemantic;
    MHWRender::MShaderInstance::ParameterType _parameterType;
    bool                                      _isVaryingInput;
};

//! List of automatic shader stage input parameters to register in VP2.
std::vector<AutomaticShaderStageInput> _automaticShaderStageInputs
    = { { MHWRender::MFragmentManager::kVertexShader,
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2ShaderFragments/shaderFragments.cpp" line="176">

---

## Shader Fragments Resource Path

The `_GetResourcePath` function is used to get the file path of the shader fragment. It uses the `PlugRegistry` to get the plugin with the name 'mayaUsd_ShaderFragments' and then uses `PlugFindPluginResource` to find the resource path for the shader fragment.

```c++
//! Get the file path of the shader fragment.
std::string _GetResourcePath(const std::string& resource)
{
    static PlugPluginPtr plugin
        = PlugRegistry::GetInstance().GetPluginWithName("mayaUsd_ShaderFragments");
    if (!TF_VERIFY(plugin, "Could not get plugin\n")) {
        return std::string();
    }

    const std::string path = PlugFindPluginResource(plugin, resource);
    TF_VERIFY(!path.empty(), "Could not find resource: %s\n", resource.c_str());

    return path;
}
```

---

</SwmSnippet>

# Shader Fragments Functions

This section will cover the main functions used in the implementation of Shader Fragments in the Maya-USD plugin.

<SwmSnippet path="/lib/mayaUsd/render/vp2ShaderFragments/shaderFragments.cpp" line="177">

---

## \_GetResourcePath Function

The `_GetResourcePath` function is used to get the file path of the shader fragment. It uses the `PlugRegistry` to get the plugin with the name 'mayaUsd_ShaderFragments' and verifies if the plugin is valid. Then it uses the `PlugFindPluginResource` function to find the resource path of the shader fragment.

```c++
std::string _GetResourcePath(const std::string& resource)
{
    static PlugPluginPtr plugin
        = PlugRegistry::GetInstance().GetPluginWithName("mayaUsd_ShaderFragments");
    if (!TF_VERIFY(plugin, "Could not get plugin\n")) {
        return std::string();
    }

    const std::string path = PlugFindPluginResource(plugin, resource);
    TF_VERIFY(!path.empty(), "Could not find resource: %s\n", resource.c_str());

    return path;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2ShaderFragments/shaderFragments.cpp" line="267">

---

## registerFragments Function

The `registerFragments` function is used to register all the shader fragments. It checks if the renderer and the fragment manager are valid. Then it registers all the fragments defined in `_LanguageSpecificFragmentNames` and `_FragmentNames`.

```c++
MStatus HdVP2ShaderFragments::registerFragments()
{
    // If we're already registered, do nothing.
    if (_registrationCount > 0) {
        _registrationCount++;
        return MS::kSuccess;
    }

    MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
    if (!theRenderer) {
        return MS::kFailure;
    }

    MHWRender::MFragmentManager* fragmentManager = theRenderer->getFragmentManager();
    if (!fragmentManager) {
        return MS::kFailure;
    }

    std::string language;

    switch (theRenderer->drawAPI()) {
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2ShaderFragments/shaderFragments.cpp" line="575">

---

## deregisterFragments Function

The `deregisterFragments` function is used to deregister all the shader fragments. It checks if the renderer and the fragment manager are valid. Then it deregisters all the fragments defined in `_FragmentNames`.

```c++
MStatus HdVP2ShaderFragments::deregisterFragments()
{
    // If it was never registered, leave as-is:
    if (_registrationCount == 0) {
        return MS::kSuccess;
    }

    // If more than one plugin still has us registered, do nothing.
    if (_registrationCount > 1) {
        _registrationCount--;
        return MS::kSuccess;
    }
    MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
    if (!theRenderer) {
        return MS::kFailure;
    }

    MHWRender::MFragmentManager* fragmentManager = theRenderer->getFragmentManager();
    if (!fragmentManager) {
        return MS::kFailure;
    }
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
