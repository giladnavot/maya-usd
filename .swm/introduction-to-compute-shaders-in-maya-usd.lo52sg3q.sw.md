---
title: Introduction to Compute Shaders in Maya-USD
---
Compute Shaders in the maya-usd repository are used to perform computations on the GPU. They are written in OpenCL and GLSL, two high-level programming languages used for writing shaders. The compute shaders in this repository are primarily used for computing normals, which are vectors perpendicular to a surface and are essential for rendering 3D graphics. The shaders take in positions and adjacency information of vertices, and output the computed normals.

<SwmSnippet path="/lib/mayaUsd/render/vp2ComputeShaders/computeNormals.cl" line="1">

---

# Compute Shaders in OpenCL

This is an example of a compute shader written in OpenCL. It takes in a set of positions and adjacency information, and computes the normals for each vertex. The normals are then stored in an output buffer.

```common lisp
__kernel void computeNormals(
    __global const float* positions,
    const unsigned int    vertexCount, // number of posisions and normals
                                       /*  Adjacency buffer is two distinct parts.
                                           First, two ints per vertex the offset and the valence. The valence is the number of adjacent
                                          vertices. The offset is the offset into the adjacency buffer to find the vertex ids of the
                                          adjacent vertices.                                    Next, a list of vertex ids of the
                                          adjacent vertices for each vertex, found                                    using the information from the first part of the
                                          buffer.
                                       */
    __global const int* adjacency,
    __global float*     normals)
{
    unsigned int vertexId = get_global_id(0);
    if (vertexId >= vertexCount)
        return;

    unsigned int  offsetIdx = vertexId * 2;
    int           offset = adjacency[offsetIdx];
    int           valence = adjacency[offsetIdx + 1];
    __global int* currAdj = &adjacency[offset];
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2ComputeShaders/computeNormals.glsl" line="1">

---

# Compute Shaders in GLSL

This is an example of a compute shader written in GLSL. It performs a similar function to the OpenCL shader, but is written in a different language. It also takes in additional data, such as rendering to scene and scene to rendering information.

```glsl
#version 430

layout( std140, binding=0 ) uniform Values
{
	uint VertexCount;
};

// This is a float3 but for buffer layout to be correct use float
layout( std430, binding=1 ) buffer Pos
{
	float Positions[ ];
};

layout( std430, binding=2 ) buffer Adj
{
	int Adjacency[ ];
};

layout( std430, binding=3 ) buffer RtoS
{
	int RenderingToScene[ ];
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2ComputeShaders/plugInfo.json" line="1">

---

# Compute Shaders Plugin Information

This file contains information about the compute shaders plugin, including its name and resource path.

```json
{
    "Plugins": [
        {
            "Name": "mayaUsd_ComputeShaders", 
            "Info": {}, 
            "ResourcePath": "../../usd/mayaUsd_ComputeShaders/resources", 
            "Root": "..", 
            "Type": "resource"
        }
    ]
}
```

---

</SwmSnippet>

# Compute Shaders Functions

Compute shaders are a type of shader that are used for performing computations that may not be directly related to rendering. They can be used for tasks such as physics simulations, image processing, and more complex mathematical tasks. In the context of the Maya-USD repository, compute shaders are primarily used for computing normals in the rendering process.

<SwmSnippet path="/lib/mayaUsd/render/vp2ComputeShaders/computeNormals.cl" line="1">

---

## computeNormals

The `computeNormals` function in the [computeNormals.cl](http://computeNormals.cl) file is a kernel function that computes the normals for a given set of vertices. It takes in the positions of the vertices, the number of vertices, and an adjacency buffer that contains information about the adjacent vertices for each vertex. The function then calculates the normal for each vertex and stores it in the normals buffer.

```common lisp
__kernel void computeNormals(
    __global const float* positions,
    const unsigned int    vertexCount, // number of posisions and normals
                                       /*  Adjacency buffer is two distinct parts.
                                           First, two ints per vertex the offset and the valence. The valence is the number of adjacent
                                          vertices. The offset is the offset into the adjacency buffer to find the vertex ids of the
                                          adjacent vertices.                                    Next, a list of vertex ids of the
                                          adjacent vertices for each vertex, found                                    using the information from the first part of the
                                          buffer.
                                       */
    __global const int* adjacency,
    __global float*     normals)
{
    unsigned int vertexId = get_global_id(0);
    if (vertexId >= vertexCount)
        return;

    unsigned int  offsetIdx = vertexId * 2;
    int           offset = adjacency[offsetIdx];
    int           valence = adjacency[offsetIdx + 1];
    __global int* currAdj = &adjacency[offset];
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/vp2ComputeShaders/computeNormals.glsl" line="1">

---

## computeNormals

The `computeNormals` function in the computeNormals.glsl file is similar to the one in [computeNormals.cl](http://computeNormals.cl), but it is written in GLSL. It also computes the normals for a given set of vertices, but it uses different buffer layouts and bindings. The function calculates the normal for each vertex and stores it in the Normals buffer.

```glsl
#version 430

layout( std140, binding=0 ) uniform Values
{
	uint VertexCount;
};

// This is a float3 but for buffer layout to be correct use float
layout( std430, binding=1 ) buffer Pos
{
	float Positions[ ];
};

layout( std430, binding=2 ) buffer Adj
{
	int Adjacency[ ];
};

layout( std430, binding=3 ) buffer RtoS
{
	int RenderingToScene[ ];
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
