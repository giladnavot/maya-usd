---
title: maya-usd overview
---
The maya-usd repository is used to create a Maya plugin and reusable libraries for Pixar's Universal Scene Description (USD). It merges the best features of Pixar's USDMaya and Animal Logic's plugins into one. It also provides installation instructions, forums for help, and detailed documentation.

- <SwmLink doc-title="Getting started">[Getting started](.swm/getting-started.p0du2hs3.sw.md)</SwmLink>

## Modules

### USD Editing

USD Editing in the maya-usd repository refers to the process of making changes to the Universal Scene Description (USD) data within the Maya environment. This can involve operations such as translating, rotating, or scaling objects, renaming prims, or modifying attributes.

- <SwmLink doc-title="Understanding USD Editing">[Understanding USD Editing](/.swm/understanding-usd-editing.mwh97kqc.sw.md)</SwmLink>
- <SwmLink doc-title="Configuring and Installing Icons with CMake in lib/mayaUsd/resources/icons">[Configuring and Installing Icons with CMake in lib/mayaUsd/resources/icons](/.swm/configuring-and-installing-icons-with-cmake-in-libmayausdresourcesicons.341o9i7n.sw.md)</SwmLink>
- <SwmLink doc-title="Overview of the API in maya-usd">[Overview of the API in maya-usd](/.swm/overview-of-the-api-in-maya-usd.yr4gt9v5.sw.md)</SwmLink>
- **File IO**
  - <SwmLink doc-title="Understanding File IO">[Understanding File IO](/.swm/understanding-file-io.26dru7i8.sw.md)</SwmLink>
  - **Translators**
    - <SwmLink doc-title="Understanding the Role of Translators">[Understanding the Role of Translators](/.swm/understanding-the-role-of-translators.0zlmc5g5.sw.md)</SwmLink>
    - <SwmLink doc-title="Translating USD Attributes to Maya Camera Attributes">[Translating USD Attributes to Maya Camera Attributes](/.swm/translating-usd-attributes-to-maya-camera-attributes.a9vtdms2.sw.md)</SwmLink>
    - <SwmLink doc-title="Light Attributes Writing Process">[Light Attributes Writing Process](/.swm/light-attributes-writing-process.gm1kb4r7.sw.md)</SwmLink>
  - **Chaser**
    - <SwmLink doc-title="Exploring the Chaser Concept">[Exploring the Chaser Concept](/.swm/exploring-the-chaser-concept.wg17myx9.sw.md)</SwmLink>
  - **Jobs**
    - <SwmLink doc-title="Understanding Job Operations">[Understanding Job Operations](/.swm/understanding-job-operations.iqxo9jjo.sw.md)</SwmLink>
  - **Shading**
    - <SwmLink doc-title="Exploring Shading in Maya-USD">[Exploring Shading in Maya-USD](/.swm/exploring-shading-in-maya-usd.m49x3m76.sw.md)</SwmLink>
    - <SwmLink doc-title="Material Binding in Maya USD">[Material Binding in Maya USD](/.swm/material-binding-in-maya-usd.s3jenjml.sw.md)</SwmLink>
    - <SwmLink doc-title="Shading Mode Export Process">[Shading Mode Export Process](/.swm/shading-mode-export-process.f0kkntcc.sw.md)</SwmLink>
  - **Utils**
    - <SwmLink doc-title="Overview of Utils in Maya-USD">[Overview of Utils in Maya-USD](/.swm/overview-of-utils-in-maya-usd.d3fxf78w.sw.md)</SwmLink>
    - <SwmLink doc-title="Creation of Maya References in Maya-USD">[Creation of Maya References in Maya-USD](/.swm/creation-of-maya-references-in-maya-usd.n54ih8nz.sw.md)</SwmLink>
    - <SwmLink doc-title="Writing RfM Light Attributes to a USD Prim">[Writing RfM Light Attributes to a USD Prim](/.swm/writing-rfm-light-attributes-to-a-usd-prim.7arnvlgs.sw.md)</SwmLink>
    - <SwmLink doc-title="Export Process of Shading Engines">[Export Process of Shading Engines](/.swm/export-process-of-shading-engines.4jho7380.sw.md)</SwmLink>
  - **Main Flows**
    - <SwmLink doc-title="Export of Shading Engines in Maya USD">[Export of Shading Engines in Maya USD](/.swm/export-of-shading-engines-in-maya-usd.smsfduiq.sw.md)</SwmLink>
    - <SwmLink doc-title="Writing RfM Light Attributes to USD Prim">[Writing RfM Light Attributes to USD Prim](/.swm/writing-rfm-light-attributes-to-usd-prim.nsj08fmo.sw.md)</SwmLink>
- **Rendering**
  - <SwmLink doc-title="Basic Concepts of Rendering">[Basic Concepts of Rendering](/.swm/basic-concepts-of-rendering.dh35myoa.sw.md)</SwmLink>
  - **Viewport 2.0 Rendering**
    - <SwmLink doc-title="Introduction to Viewport 2.0 Rendering">[Introduction to Viewport 2.0 Rendering](/.swm/introduction-to-viewport-20-rendering.f8igoagv.sw.md)</SwmLink>
    - <SwmLink doc-title="Overview of Shader Fragments">[Overview of Shader Fragments](/.swm/overview-of-shader-fragments.grxmq5hn.sw.md)</SwmLink>
    - <SwmLink doc-title="Introduction to Compute Shaders in Maya-USD">[Introduction to Compute Shaders in Maya-USD](/.swm/introduction-to-compute-shaders-in-maya-usd.lo52sg3q.sw.md)</SwmLink>
    - **Render Delegate**
      - <SwmLink doc-title="Introduction to Render Delegate">[Introduction to Render Delegate](/.swm/introduction-to-render-delegate.5p3a0kfb.sw.md)</SwmLink>
      - <SwmLink doc-title="VP2 State Synchronization Process">[VP2 State Synchronization Process](/.swm/vp2-state-synchronization-process.b3kmir00.sw.md)</SwmLink>
      - <SwmLink doc-title="Material Texture Loading Process">[Material Texture Loading Process](/.swm/material-texture-loading-process.k3700pym.sw.md)</SwmLink>
      - <SwmLink doc-title="Synchronization of Shared Data">[Synchronization of Shared Data](/.swm/synchronization-of-shared-data.08rfndsc.sw.md)</SwmLink>
      - <SwmLink doc-title="Mesh Rendering in Maya Viewport">[Mesh Rendering in Maya Viewport](/.swm/mesh-rendering-in-maya-viewport.2w9sffey.sw.md)</SwmLink>
    - **Main Flows**
      - <SwmLink doc-title="Shader Synchronization and Management">[Shader Synchronization and Management](/.swm/shader-synchronization-and-management.uh4vgztx.sw.md)</SwmLink>
      - <SwmLink doc-title="Material ID Update Process">[Material ID Update Process](/.swm/material-id-update-process.3yh15n6b.sw.md)</SwmLink>
      - <SwmLink doc-title="Flow of Function Calls in Maya USD Plugin">[Flow of Function Calls in Maya USD Plugin](/.swm/flow-of-function-calls-in-maya-usd-plugin.752no1e4.sw.md)</SwmLink>
  - **MaterialX Generation**
    - <SwmLink doc-title="Getting Started with MaterialX Generation">[Getting Started with MaterialX Generation](/.swm/getting-started-with-materialx-generation.bq7ic0ge.sw.md)</SwmLink>
    - <SwmLink doc-title="Introduction to MaterialX Libraries">[Introduction to MaterialX Libraries](/.swm/introduction-to-materialx-libraries.6jfk2v02.sw.md)</SwmLink>
    - <SwmLink doc-title="Understanding MaterialX Nodes in Maya-USD">[Understanding MaterialX Nodes in Maya-USD](/.swm/understanding-materialx-nodes-in-maya-usd.mu3z0npa.sw.md)</SwmLink>
    - <SwmLink doc-title="Exploring Shader Generation">[Exploring Shader Generation](/.swm/exploring-shader-generation.ughh72vj.sw.md)</SwmLink>
  - **Maya to Hydra Rendering**
    - <SwmLink doc-title="Getting Started with Maya to Hydra Rendering">[Getting Started with Maya to Hydra Rendering](/.swm/getting-started-with-maya-to-hydra-rendering.ol6xzdpk.sw.md)</SwmLink>
    - **Maya to Hydra Rendering**
      - <SwmLink doc-title="Overview of Maya to Hydra Rendering">[Overview of Maya to Hydra Rendering](/.swm/overview-of-maya-to-hydra-rendering.2m39llfg.sw.md)</SwmLink>
      - <SwmLink doc-title="Rendering in Maya to Hydra Plugin">[Rendering in Maya to Hydra Plugin](/.swm/rendering-in-maya-to-hydra-plugin.jpld5idb.sw.md)</SwmLink>
    - **PxrUsdMayaGL**
      - <SwmLink doc-title="Exploring PxrUsdMayaGL">[Exploring PxrUsdMayaGL](/.swm/exploring-pxrusdmayagl.dxl6gp5u.sw.md)</SwmLink>
      - <SwmLink doc-title="Draw Requests Generation Process">[Draw Requests Generation Process](/.swm/draw-requests-generation-process.bfcet8vx.sw.md)</SwmLink>
      - <SwmLink doc-title="Prepare for Draw Process">[Prepare for Draw Process](/.swm/prepare-for-draw-process.izj8b8x2.sw.md)</SwmLink>
      - <SwmLink doc-title="Rendering Process in Maya Plugin">[Rendering Process in Maya Plugin](/.swm/rendering-process-in-maya-plugin.7xbo0lgw.sw.md)</SwmLink>
  - **Main Flows**
    - <SwmLink doc-title="VP2 State Synchronization Process">[VP2 State Synchronization Process](/.swm/vp2-state-synchronization-process.11oi74zp.sw.md)</SwmLink>
    - <SwmLink doc-title="Shape Syncing and Rendering Process">[Shape Syncing and Rendering Process](/.swm/shape-syncing-and-rendering-process.22neby0c.sw.md)</SwmLink>
    - <SwmLink doc-title="Rendering Process in Maya Plugin">[Rendering Process in Maya Plugin](/.swm/rendering-process-in-maya-plugin.ofrmov8m.sw.md)</SwmLink>
    - <SwmLink doc-title="Material ID Updating Process">[Material ID Updating Process](/.swm/material-id-updating-process.eqoipgro.sw.md)</SwmLink>
    - <SwmLink doc-title="Rendering Process in Maya to Hydra">[Rendering Process in Maya to Hydra](/.swm/rendering-process-in-maya-to-hydra.tb4hohyl.sw.md)</SwmLink>
- **Commands**
  - <SwmLink doc-title="Understanding Commands in Maya-USD">[Understanding Commands in Maya-USD](/.swm/understanding-commands-in-maya-usd.7uh72omp.sw.md)</SwmLink>
  - <SwmLink doc-title="Role of the doIt, undoIt, and redoIt Functions in a Command Class">[Role of the doIt, undoIt, and redoIt Functions in a Command Class](/.swm/role-of-the-doit-undoit-and-redoit-functions-in-a-command-class.7lugpa7k.sw.md)</SwmLink>
  - <SwmLink doc-title="Argument Parsing in Commands using MSyntax and MArgDatabase">[Argument Parsing in Commands using MSyntax and MArgDatabase](/.swm/argument-parsing-in-commands-using-msyntax-and-margdatabase.w6z0blwo.sw.md)</SwmLink>
- **Main Flows**
  - <SwmLink doc-title="Synchronization of VP2 State in Maya USD">[Synchronization of VP2 State in Maya USD](/.swm/synchronization-of-vp2-state-in-maya-usd.fo7w3fb6.sw.md)</SwmLink>
  - <SwmLink doc-title="Creation of a Maya Reference">[Creation of a Maya Reference](/.swm/creation-of-a-maya-reference.evo2wbl4.sw.md)</SwmLink>

### Maya Plugin

The Maya Plugin in the maya-usd repository is a piece of software that adds specific capabilities to the Autodesk Maya software. It is designed to integrate Pixar's Universal Scene Description (USD) into Maya, allowing for efficient collaboration and interchange of 3D graphics data.&nbsp;

- <SwmLink doc-title="Understanding the Maya Plugin">[Understanding the Maya Plugin](/.swm/understanding-the-maya-plugin.4247d7an.sw.md)</SwmLink>
- **pxr Plugin**
  - <SwmLink doc-title="Understanding the pxr Plugin">[Understanding the pxr Plugin](/.swm/understanding-the-pxr-plugin.ujn6823b.sw.md)</SwmLink>
  - <SwmLink doc-title="Overview of USD Maya Plugin">[Overview of USD Maya Plugin](/.swm/overview-of-usd-maya-plugin.kd60ikqn.sw.md)</SwmLink>
- **al Plugin**
  - <SwmLink doc-title="Getting Started with the AL Plugin">[Getting Started with the AL Plugin](/.swm/getting-started-with-the-al-plugin.dfgxozpe.sw.md)</SwmLink>
  - **al Translators**
    - <SwmLink doc-title="Getting Started with AL Translators">[Getting Started with AL Translators](/.swm/getting-started-with-al-translators.kim2ebj7.sw.md)</SwmLink>
    - <SwmLink doc-title="Introduction to Translators">[Introduction to Translators](/.swm/introduction-to-translators.l3fei4ix.sw.md)</SwmLink>
  - **al Schemas**
    - <SwmLink doc-title="Getting Started with al Schemas">[Getting Started with al Schemas](/.swm/getting-started-with-al-schemas.d2se5d9e.sw.md)</SwmLink>
    - <SwmLink doc-title="Understanding the MayaTest Component">[Understanding the MayaTest Component](/.swm/understanding-the-mayatest-component.uiqgttvj.sw.md)</SwmLink>
  - **al Utils**
    - <SwmLink doc-title="Exploring the al Utils Directory">[Exploring the al Utils Directory](/.swm/exploring-the-al-utils-directory.gawj7ql4.sw.md)</SwmLink>
    - <SwmLink doc-title="Exploring the Event Handler">[Exploring the Event Handler](/.swm/exploring-the-event-handler.jxdz4t7k.sw.md)</SwmLink>
- **adsk Plugin**
  - <SwmLink doc-title="Overview of the adsk Plugin">[Overview of the adsk Plugin](/.swm/overview-of-the-adsk-plugin.v72aua66.sw.md)</SwmLink>

## Classes

- <SwmLink doc-title="UsdMayaShaderReader Class Overview">[UsdMayaShaderReader Class Overview](/.swm/usdmayashaderreader-class-overview.7biyn.sw.md)</SwmLink>
- <SwmLink doc-title="MtlxUsd_BaseWriter Overview">[MtlxUsd_BaseWriter Overview](/.swm/mtlxusd_basewriter-overview.fz9cn.sw.md)</SwmLink>
- <SwmLink doc-title="Overview of the UsdUndoableCommand Class">[Overview of the UsdUndoableCommand Class](/.swm/overview-of-the-usdundoablecommand-class.0psbr.sw.md)</SwmLink>
- <SwmLink doc-title="TranslatorBase Class Overview">[TranslatorBase Class Overview](/.swm/translatorbase-class-overview.vxvfd.sw.md)</SwmLink>

## Build Tools

- <SwmLink doc-title="CMake Configuration in Maya-USD">[CMake Configuration in Maya-USD](/.swm/cmake-configuration-in-maya-usd.q89qwmgw.sw.md)</SwmLink>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](https://app.swimm.io/)</sup></SwmMeta>
