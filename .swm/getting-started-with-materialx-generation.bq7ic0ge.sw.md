---
title: Getting Started with MaterialX Generation
---
MaterialX Generation in the maya-usd repository refers to the process of creating MaterialX documents and shaders for rendering in Maya. This involves generating XML fragment sources that embed both GLSL and HLSL code, which are used for rendering in different graphics APIs. The MaterialX documents are created and manipulated using functions like `getDocument` and `setCommonOptions`. The generation process also involves setting various options for the MaterialX shaders, such as the number of samples, the method for rendering specular environments, and the method for rendering transmission. These options are set based on the capabilities of the Maya environment and user-defined option variables. The MaterialX Generation process also involves the use of various helper functions and utilities defined in the `ShaderGenUtil` namespace.

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/OgsFragment.h" line="46">

---

# MaterialX Document Creation

The `getDocument` function is used to retrieve the MaterialX document associated with a particular shader. This document describes the appearance of the object that the shader will be applied to.

```c
    /// Return the MaterialX document.
    mx::DocumentPtr getDocument() const
    {
        return _element ? _element->getDocument() : mx::DocumentPtr();
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/GlslFragmentGenerator.cpp" line="262">

---

# MaterialX GLSL Code Generation

The `generate` function is used to generate the GLSL code for a shader from a MaterialX document. It creates a shader, sets up the necessary parameters and options for rendering, and then generates the GLSL code.

```c++
ShaderPtr GlslFragmentGenerator::generate(
    const string& fragmentName,
    ElementPtr    element,
    GenContext&   context) const
{
    ShaderPtr shader = createShader(fragmentName, element, context);

    ShaderStage& pixelStage = shader->getStage(Stage::PIXEL);
    ShaderGraph& graph = shader->getGraph();

    // Turn on fixed float formatting to make sure float values are
    // emitted with a decimal point and not as integers, and to avoid
    // any scientific notation which isn't supported by all OpenGL targets.
    ScopedFloatFormatting floatFormatting(Value::FloatFormatFixed);

    const VariableBlock& vertexData = pixelStage.getInputBlock(HW::VERTEX_DATA);

    // Add global constants and type definitions
    const unsigned int maxLights = std::max(1u, context.getOptions().hwMaxActiveLightSources);
    emitLine("#define MAX_LIGHT_SOURCES " + std::to_string(maxLights), pixelStage, false);
    emitLineBreak(pixelStage);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/OgsFragment.cpp" line="115">

---

# MaterialX Rendering Options Configuration

The `setCommonOptions` function is used to configure the rendering options for a shader. This includes settings such as the number of samples to use for rendering, whether to use Monte Carlo sampling, and how to handle transparency.

```c++
    void setCommonOptions(
        mx::GenOptions&            genOptions,
        mx::GenContext&            context,
        const mx::ShaderGenerator& generator)
    {
        int  numSamples = 64;
        bool isMonteCarlo = false;
        genOptions.hwSpecularEnvironmentMethod = _getEnvironmentOptions(numSamples, isMonteCarlo);
        // FIS option has further sub-options to check:
        if (genOptions.hwSpecularEnvironmentMethod == mx::SPECULAR_ENVIRONMENT_FIS) {
            context.pushUserData(
                mx::HwSpecularEnvironmentSamples::name(),
                mx::HwSpecularEnvironmentSamples::create(numSamples));
            if (isMonteCarlo) {
                genOptions.hwDirectionalAlbedoMethod = mx::DIRECTIONAL_ALBEDO_MONTE_CARLO;
            }
        }

#if MX_COMBINED_VERSION >= 13805
        // MaterialX has a new implementation of transmission as refraction in version 1.38.5, but
        // does not work out of the box in Maya (probably because we only output a color).
```

---

</SwmSnippet>

# MaterialX Generation Endpoints

MaterialX Generation Endpoints

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/ShaderGenUtil.h" line="34">

---

## getDocument

The `getDocument` method in the `TopoNeutralGraph` class returns a pointer to the MaterialX document that is being used. This document contains the material definitions and the shading graph that is being processed.

```c
    mx::DocumentPtr           getDocument() const;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/OgsFragment.h" line="53">

---

## getShader

The `getShader` method in the `OgsFragment` class returns a pointer to the GLSL shader that is generated for the fragment. This shader code is used by the rendering engine to render the material on the geometry in the scene.

```c
    mx::ShaderPtr getShader() const { return _glslShader; }
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
