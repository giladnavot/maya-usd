---
title: Basic Concepts of Asset Import/Export
---

Asset Import/Export in the context of the maya-usd repository refers to the process of bringing in (import) or sending out (export) 3D models or scenes, often saved in file formats such as .ma or .usda. These assets can include a variety of data such as meshes, materials, lights, animations, and more. The imported assets can then be manipulated within Maya, and once the desired changes are made, they can be exported back into USD for use in other applications or further stages of the pipeline.

# Importing Assets

This is an example of importing an asset into a Maya scene. The `pxrUsdReferenceAssembly` node is used to reference an external USD file (the asset). The asset can then be manipulated as a single object in the scene.

# Exporting Assets

This is an example of an asset that has been exported from Maya. The file describes a 'CubeModel' asset in the USD format. This asset can be imported into any software that supports USD, not just Maya.

# Asset Import/Export

The Asset Import/Export functionality in Maya-USD allows for the conversion and manipulation of 3D scene data between Maya and USD formats.

## Maya ASCII Scene

This Maya ASCII scene file demonstrates the creation of a scene with various nodes. The 'pxrUsdReferenceAssembly' nodes (lines 74, 82, 91) are particularly relevant to USD, as they represent references to USD assets.

## USD Scene

This USD file defines a 3D scene with a cube model. The cube model has different shading variants (lines 30-59), demonstrating the ability to define multiple visual appearances for a single model in USD.

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
