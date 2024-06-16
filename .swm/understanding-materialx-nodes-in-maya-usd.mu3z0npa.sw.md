---
title: Understanding MaterialX Nodes in Maya-USD
---
MaterialX Nodes in the maya-usd repository refer to the various nodes used in the MaterialX shading language for creating shaders in the Maya environment. These nodes, defined in the MaterialXGenOgsXml/Nodes directory, include different types of nodes such as SurfaceNodeMaya, TexcoordNodeMaya, and MayaTransformVectorNodeGlsl, among others. Each node serves a specific purpose in the shader generation process. For instance, TexcoordNodeMaya is used for handling texture coordinates, while SurfaceNodeMaya is used for defining surface shaders. These nodes are implemented using the MaterialX and GLSL (OpenGL Shading Language) libraries.

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/TexcoordNodeMaya.h" line="6">

---

# Texture Coordinate Node

The `TexcoordNodeMaya` class is a MaterialX Node that handles texture coordinates. It is defined in the `TexcoordNodeMaya.h` header file.

```c
#ifndef MATERIALX_TEXCOORDNODEGLSLMAYA_H
#define MATERIALX_TEXCOORDNODEGLSLMAYA_H

#include <MaterialXGenGlsl/GlslShaderGenerator.h>
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/MayaHwImageNode.cpp" line="25">

---

# Image Node

The `MayaHwImageNode` class is a MaterialX Node that handles image textures. It uses the `UV_SCALE` constant to scale the UV coordinates for the image texture.

```c++
    // Add additional scale and offset inputs to match implementation arguments
    ShaderInput* scaleInput = node.addInput(UV_SCALE, Type::VECTOR2);
    scaleInput->setValue(Value::createValue<Vector2>(Vector2(1.0f, 1.0f)));
    if (ncGraphNode) {
        ncGraphNode->addPropagatedInput(node, UV_SCALE);
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/MayaCompoundNode.h" line="6">

---

# Compound Node

The `MayaCompoundNode` class is a MaterialX Node that represents a compound node, which is a node that can contain other nodes. It is defined in the `MayaCompoundNode.h` header file.

```c
#ifndef MAYA_MATERIALX_COMPOUNDNODE_H
#define MAYA_MATERIALX_COMPOUNDNODE_H

#include <MaterialXGenShader/Nodes/CompoundNode.h>
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/SurfaceNodeMaya.h" line="6">

---

# Surface Node

The `SurfaceNodeMaya` class is a MaterialX Node that handles surface shading. It is defined in the `SurfaceNodeMaya.h` header file.

```c
#ifndef MATERIALX_SURFACENODEMAYA_H
#define MATERIALX_SURFACENODEMAYA_H

#include <MaterialXGenGlsl/GlslShaderGenerator.h>
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/MayaTransformVectorNodeGlsl.h" line="6">

---

# Transform Vector Node

The `MayaTransformVectorNodeGlsl` class is a MaterialX Node that handles vector transformations. It is defined in the `MayaTransformVectorNodeGlsl.h` header file.

```c
#ifndef MATERIALX_MAYATRANSFORMVECTORNODEGLSL_H
#define MATERIALX_MAYATRANSFORMVECTORNODEGLSL_H

#include <MaterialXGenGlsl/GlslShaderGenerator.h>
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/MayaTransformNormalNodeGlsl.h" line="6">

---

# Transform Normal Node

The `MayaTransformNormalNodeGlsl` class is a MaterialX Node that handles normal vector transformations. It is defined in the `MayaTransformNormalNodeGlsl.h` header file.

```c
#ifndef MATERIALX_MAYATRANSFORMNORMALNODEGLSL_H
#define MATERIALX_MAYATRANSFORMNORMALNODEGLSL_H

#include "MayaTransformVectorNodeGlsl.h"
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/MayaTransformPointNodeGlsl.h" line="6">

---

# Transform Point Node

The `MayaTransformPointNodeGlsl` class is a MaterialX Node that handles point transformations. It is defined in the `MayaTransformPointNodeGlsl.h` header file.

```c
#ifndef MATERIALX_MAYATRANSFORMPOINTNODEGLSL_H
#define MATERIALX_MAYATRANSFORMPOINTNODEGLSL_H

#include "MayaTransformVectorNodeGlsl.h"
```

---

</SwmSnippet>

# MaterialX Nodes Functions

This section will cover the functions of MaterialX Nodes in the maya-usd repository.

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/MayaCompoundNode.cpp" line="16">

---

## MayaCompoundNode Functions

The `initialize` function in the `MayaCompoundNode` class is used to set up the node. It takes an `InterfaceElement` and a `GenContext` as parameters. The function sets the name of the node, creates a `MayaShaderGraph`, and sets the hash of the node using the function name.

```c++
void MayaCompoundNode::initialize(const InterfaceElement& element, GenContext& context)
{
    // Copied directly from base class

    ShaderNodeImpl::initialize(element, context);

    if (!element.isA<NodeGraph>()) {
        throw ExceptionShaderGenError(
            "Element '" + element.getName() + "' is not a node graph implementation");
    }

    const NodeGraph& graph = static_cast<const NodeGraph&>(element);

    _functionName = graph.getName();
    context.getShaderGenerator().getSyntax().makeValidName(_functionName);

    // For compounds we do not want to publish all internal inputs
    // so always use the reduced interface for this graph.
    const ShaderInterfaceType oldShaderInterfaceType = context.getOptions().shaderInterfaceType;
    context.getOptions().shaderInterfaceType = SHADER_INTERFACE_REDUCED;

```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/MayaCompoundNode.cpp" line="46">

---

The `addInputs` function in the `MayaCompoundNode` class is used to add inputs to the node. It takes a `ShaderNode` and a `GenContext` as parameters.

```c++
void MayaCompoundNode::addInputs(ShaderNode& node, GenContext& context) const
{
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/TexcoordNodeMaya.cpp" line="49">

---

## TexcoordNodeGlslMaya Functions

The `create` function in the `TexcoordNodeGlslMaya` class is used to create a new instance of the class.

```c++
ShaderNodeImplPtr TexcoordNodeGlslMaya::create()
{
    return std::make_shared<TexcoordNodeGlslMaya>();
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/TexcoordNodeMaya.cpp" line="54">

---

The `createVariables` function in the `TexcoordNodeGlslMaya` class is used to create variables for the node. It takes a `ShaderNode`, a `GenContext`, and a `Shader` as parameters. The function adds inputs to the vertex stage of the shader.

```c++
void TexcoordNodeGlslMaya::createVariables(const ShaderNode& node, GenContext&, Shader& shader)
    const
{
    const ShaderInput* indexInput = node.getInput(INDEX);
    if (!indexInput || !indexInput->getValue()) {
        throw ExceptionShaderGenError(
            "No 'index' parameter found on texcoord node '" + node.getName()
            + "'. Don't know what property to bind");
    }
    // Use the standard USD convention for texcoord primvar names:
    const string        index = indexInput ? indexInput->getValue()->getValueString() : "0";
    const string        geomProp = geomNameFromIndex(index);
    const ShaderOutput* output = node.getOutput();

    ShaderStage& vs = shader.getStage(Stage::VERTEX);
    ShaderStage& ps = shader.getStage(Stage::PIXEL);

    addStageInput(HW::VERTEX_INPUTS, output->getType(), HW::T_IN_GEOMPROP + "_" + geomProp, vs);
    addStageConnector(
        HW::VERTEX_DATA, output->getType(), HW::T_IN_GEOMPROP + "_" + geomProp, vs, ps);
}
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/Nodes/TexcoordNodeMaya.cpp" line="76">

---

The `emitFunctionCall` function in the `TexcoordNodeGlslMaya` class is used to emit a function call for the node. It takes a `ShaderNode`, a `GenContext`, and a `ShaderStage` as parameters. The function emits code for the vertex and pixel stages of the shader.

```c++
void TexcoordNodeGlslMaya::emitFunctionCall(
    const ShaderNode& node,
    GenContext&       context,
    ShaderStage&      stage) const
{
    const GlslShaderGenerator& shadergen
        = static_cast<const GlslShaderGenerator&>(context.getShaderGenerator());

    const ShaderInput* indexInput = node.getInput(INDEX);
    if (!indexInput) {
        throw ExceptionShaderGenError(
            "No 'index' parameter found on texcoord node '" + node.getName()
            + "'. Don't know what property to bind");
    }
    // Use the standard USD convention for texcoord primvar names:
    const string index = indexInput ? indexInput->getValue()->getValueString() : "0";
    const string geomname = geomNameFromIndex(index);
    const string variable = HW::T_IN_GEOMPROP + "_" + geomname;

#if MX_COMBINED_VERSION >= 13807
    DEFINE_SHADER_STAGE(stage, Stage::VERTEX)
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
