---
title: Exploring Shader Generation
---
Shader Generation in the maya-usd repository refers to the process of creating shaders for rendering 3D graphics. Shaders are small programs that run on the GPU and determine how to render graphics on the screen. In the context of this repository, shaders are generated using the MaterialX library, which provides a standard way to define materials and other visual effects. The shader generation process involves creating a shader graph, which is a network of nodes that define the operations to be performed by the shader. This graph is then translated into GLSL (OpenGL Shading Language) code, which can be executed by the GPU. The shader generation process also involves handling of textures and lighting, which are crucial for achieving realistic rendering results.

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/ShaderGenUtil.h" line="1">

---

# ShaderGenUtil Class

The `ShaderGenUtil` class provides utilities for shader generation. It includes a `TopoNeutralGraph` class that creates a topologically neutral copy of a shading graph. This class also provides methods to get the document, material name, and watch list.

```c
#ifndef MATERIALX_MAYA_SHADERGENUTIL_H
#define MATERIALX_MAYA_SHADERGENUTIL_H

/// @file
/// Helpers

#include <mayaUsd/base/api.h>

#include <MaterialXCore/Document.h>

#include <map>

namespace mx = MaterialX;
namespace MaterialXMaya {
namespace ShaderGenUtil {

/// Topo-neutral graph duplicator. Creates a topologically neutral copy of a shading graph:

class MAYAUSD_CORE_PUBLIC TopoNeutralGraph
{
public:
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/ShaderGenUtil.h" line="38">

---

# ElementType Enum

The `ElementType` enum class is used to differentiate between regular and topological elements in the shader graph. This is used while traversing the shader graph.

```c
    // As we traverse the shader graph, remember all elements that should be watched for value
    // changes
    enum class ElementType
    {
        eRegular,
        eTopological
    };
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/MayaShaderGraph.cpp" line="30">

---

# MayaShaderGraph Class

The `MayaShaderGraph` class represents a shader graph in Maya. It creates input sockets from the node definition and output sockets from the node graph. It also traverses all outputs to create internal nodes.

```c++
//
// ShaderGraph methods
//
MayaShaderGraph::MayaShaderGraph(
    const ShaderGraph* parent,
    const NodeGraph&   nodeGraph,
    GenContext&        context)
    : ShaderGraph(
        parent,
        makeValidName(nodeGraph, context),
        nodeGraph.getDocument(),
        context.getReservedWords())
{
    // Clear classification
    _classification = 0;

    // Create input sockets from the nodedef
    addInputSockets(*nodeGraph.getNodeDef(), context);

    // Create output sockets from the nodegraph
    addOutputSockets(nodeGraph);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/OgsXmlGenerator.cpp" line="360">

---

# generate Function

The `generate` function generates the complete XML fragment source embedding both GLSL and HLSL code. It creates the interface using one of the shaders and adds properties and values to the XML fragment.

```c++
string OgsXmlGenerator::generate(
    const std::string& shaderName,
    const Shader&      glslShader,
    const std::string& hlslSource)
{
    // Create the interface using one of the shaders (interface should be the same)
    const ShaderStage& glslPixelStage = glslShader.getStage(Stage::PIXEL);

    xml_document xmlDocument;

    pugi::xml_node xmlRoot = xmlDocument.append_child(FRAGMENT);
    xmlRoot.append_attribute(UI_NAME) = shaderName.c_str();
    xmlRoot.append_attribute(NAME) = shaderName.c_str();
    xmlRoot.append_attribute(TYPE) = PLUMBING;
    xmlRoot.append_attribute(CLASS) = SHADER_FRAGMENT;
    xmlRoot.append_attribute(VERSION) = "1";

    pugi::xml_node xmlDescription = xmlRoot.append_child(DESCRIPTION);
    xmlDescription.append_child(pugi::node_cdata)
        .set_value("Code generated from MaterialX description");

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/OgsFragment.cpp" line="439">

---

# generateLightRig Function

The `generateLightRig` function generates a fragment graph linking Maya lights to the generated fragment. It adds fragments, connects fragments, adds properties and values, and adds a color3 output.

```c++
        const mx::VariableBlock& uniforms = *uniformBlock.second;

        // Skip light uniforms
        if (uniforms.getName() == mx::HW::LIGHT_DATA) {
            continue;
        }

        for (size_t i = 0; i < uniforms.size(); ++i) {
            const mx::ShaderPort* const port = uniforms[i];
            if (!port->getNode()) {
                continue;
            }
            std::string path = port->getPath();
            // MaterialX 1.38.10 will return empty path for the root shader.
            // work around that. We know there is an associated node. No help
            // from the Node* in the port though since this will be the N0
            // ShaderGraph.
            const std::string& variableName = port->getVariable();
            if (path.empty() && surfaceInputNames.count(variableName)) {
                // Assume it is the surface shader:
                path.reserve(surfaceNodeName.size() + 1 + variableName.size());
```

---

</SwmSnippet>

# Shader Generation Functions

This section will cover the main functions involved in Shader Generation within the 'maya-usd' repository.

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/ShaderGenUtil.h" line="23">

---

## TopoNeutralGraph

The `TopoNeutralGraph` function is a topologically neutral graph duplicator. It creates a topologically neutral copy of a shading graph. This function is part of the ShaderGenUtil namespace within the MaterialXMaya namespace.

```c
    ~TopoNeutralGraph() = default;

    TopoNeutralGraph() = delete;

    TopoNeutralGraph(const TopoNeutralGraph&) = default;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/ShaderGenUtil.cpp" line="198">

---

## isTopologicalNodeDef

The `isTopologicalNodeDef` function checks if a node is topological. This is usually used for nodes that have custom shader code that varies when input value varies.

```c++
bool TopoNeutralGraph::isTopologicalNodeDef(const mx::NodeDef& nodeDef)
{
    // This is where we need to remove all these hardcoded names and instead ask the shadergen about
    // the info. Requires a shadergen that can tell if a node is topological (usually C++ nodes that
    // have custom shader code that varies when input value varies)

    // This is the hardcoded list for the GLSL shader generator:
#if MX_COMBINED_VERSION >= 13807
    // Dot filename is always topological to prevent creating extra OpenGL samplers in the
    // generated OpenGL code.
    if (nodeDef.getName() == "ND_dot_filename")
        return true;
#endif
    return _mtlxTopoNodeSet.find(nodeDef.getNodeString()) != _mtlxTopoNodeSet.cend();
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/ShaderGenUtil.cpp" line="216">

---

## findNodeGraphOutput

The `findNodeGraphOutput` function is used to find the output of a NodeGraph. It takes an input and an output name as parameters and returns the output of the NodeGraph.

```c++
mx::OutputPtr
TopoNeutralGraph::findNodeGraphOutput(const mx::Input& input, const std::string& outputName)
{
    auto sourceNode = input.getParent();
    if (!sourceNode || (!sourceNode->isA<mx::Node>() && !sourceNode->isA<mx::NodeGraph>())) {
        return nullptr;
    }

    auto scope = sourceNode->getParent();
    if (!scope || !scope->isA<mx::Document>()) {
        return nullptr;
    }

    auto nodeGraph = scope->asA<mx::Document>()->getNodeGraph(input.getNodeGraphString());
    if (!nodeGraph) {
        return nullptr;
    }

    return nodeGraph->getOutput(outputName);
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/ShaderGenUtil.cpp" line="383">

---

## getNodeGraph

The `getNodeGraph` function is used to get the NodeGraph. If the NodeGraph does not currently exist, it initializes it.

```c++
mx::NodeGraphPtr& TopoNeutralGraph::getNodeGraph()
{
    if (!_nodeGraph) {
        _nodeGraph = _doc->addNodeGraph("NG0");
    }
    return _nodeGraph;
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/GlslFragmentGenerator.cpp" line="262">

---

## generate

The `generate` function is used to generate the shader. It takes a name, an element, and a context as parameters and returns a shader.

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

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/OgsFragment.cpp" line="333">

---

## generateLightRig

The `generateLightRig` function is used to generate a fragment graph that integrates Maya lighting. It takes a shader and a base fragment name as parameters.

```c++
/// Generate a fragment graph linking Maya lights to the generated fragment:
/// @return The unique name of the fragment
std::string generateLightRig(
    std::string&       lightRigSource,
    const mx::Shader&  glslShader,
    const std::string& baseFragmentName)
{
    static const std::string FRAGMENT_NAME_TOKEN = "$fragmentName";
    static const std::string BASE_FRAGMENT_NAME_TOKEN = "$baseFragmentName";

    {
        // Supply a placeholder name token to be replaced with an actual unique
        // name later.
        //
        lightRigSource = mx::OgsXmlGenerator::generateLightRig(
            FRAGMENT_NAME_TOKEN, BASE_FRAGMENT_NAME_TOKEN, glslShader);
        if (lightRigSource.empty()) {
            throw mx::Exception("Generated light rig is empty");
        }
    }

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/SurfaceNodeMaya.cpp" line="24">

---

## createVariables

The `createVariables` function is used to create variables for the shader. It takes a node, a context, and a shader as parameters.

```c++
void SurfaceNodeMaya::createVariables(const ShaderNode& node, GenContext& context, Shader& shader)
    const
{
    SurfaceNodeGlsl::createVariables(node, context, shader);
    ShaderStage& ps = shader.getStage(Stage::PIXEL);

    addStageUniform(HW::PRIVATE_UNIFORMS, Type::INTEGER, MX_MAYA_EXTERNAL_LIGHTS, ps);
}
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
