---
title: Introduction to MaterialX Libraries
---
MaterialX Libraries in the maya-usd repository are collections of GLSL and MaterialX (mtlx) files that define various rendering and shading functionalities. These libraries are used in the MaterialXGenOgsXml rendering pipeline of the Maya plugin. They include shader definitions, color conversion utilities, and texture mapping utilities. The libraries are versioned, as seen with the 1.38.7 and 1.38.3 directories, to support different versions of MaterialX. The GLSL files define shader functions, while the mtlx files define node structures for the MaterialX shading language.

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/libraries/maya_surfaces.mtlx" line="78">

---

# Usage of MaterialX Libraries

This comment indicates that the MaterialX Libraries are used to create a minimal MaterialX graph that closely represents the desired material properties.

```mtlx
  <!-- They will use the smallest MaterialX graph that is close enough.         -->
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/libraries/maya_utilities.mtlx" line="53">

---

This comment suggests that MaterialX Libraries are used to inline the MaterialX image node, which is a common practice in many DCC applications.

```mtlx
       NOTE: A better solution would be to also inline the MaterialX image node
             in these nodegraphs to have everything under a single node.

        BUT: A lot of DCC consider that only two MaterialX nodes provide image
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/libraries/maya_utilities.mtlx" line="144">

---

This comment explains that instead of replicating the Maya noise exactly, a MaterialX native noise node is used, demonstrating the flexibility and adaptability of MaterialX Libraries.

```mtlx
  <!-- note that replicating the Maya noise exactly would require use of a
       3D texture, which will be hard to port to other render languages, so
       we will pick a MaterialX native noise node instead                       -->
```

---

</SwmSnippet>

# MaterialX Libraries Overview

The MaterialX Libraries in the maya-usd repository are a collection of scripts and shaders used for rendering and shading in the Maya plugin.

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/libraries/1.38.7/mx_srgb_texture_to_aces_2065_1_color3.glsl" line="1">

---

## MaterialX Libraries

The MaterialX Libraries include GLSL scripts like `mx_srgb_texture_to_aces_2065_1_color3.glsl`. These scripts define specific shading behaviors, such as converting sRGB textures to ACES 2065-1 color space.

```glsl
#include "libraries/stdlib/genglsl/lib/mx_transform_color.glsl"

void mx_srgb_texture_to_aces_2065_1_color3(vec3 _in, out vec3 result)
{
    result = mx_srgb_texture_to_lin_rec709(_in);

    // Same matrix as found in ../../vp2ShaderFragments/shaderFragments.cpp
    result = mat3(0.43963298, 0.08977644, 0.01754117,
                  0.38298870, 0.81343943, 0.11154655,
                  0.17737832, 0.09678413, 0.87091228) * result;
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/libraries/lookdevKit.mtlx" line="23">

---

The libraries also include MaterialX scripts like `lookdevKit.mtlx`. This script defines a `LdkColorCorrect` node for color correction in the LookdevKit plugin.

```mtlx
  <!-- colorCorrect -->
  <nodedef name="LdkND_ColorCorrect_color4" node="LdkColorCorrect" nodegroup="adjustment" doc="Color correct from Maya LookdevKit" version="1.0" isdefaultversion="true">
    <!-- Inputs -->
    <input name="inColor" type="color3" value="0.3, 0.3, 0.3" uiname="Color" uifolder="Inputs" />
    <input name="inAlpha" type="float" value="1.0" uiname="Alpha" uifolder="Inputs" />

    <!-- Color Correct -->
    <input name="hueShift" type="float" value="0.0" uimin="0.0" uisoftmax="360.0" uiname="Hue Shift" uifolder="Color Correct" />
    <input name="satGain" type="float" value="1.0" uimin="0.0" uisoftmax="1.0" uiname="Saturation" uifolder="Color Correct" />
    <input name="valGain" type="float" value="1.0" uimin="0.0" uisoftmax="1.0" uiname="Value" uifolder="Color Correct" />
    <input name="colGain" type="color3" value="1.0, 1.0, 1.0" uiname="Color Gain" uifolder="Color Correct" />
    <input name="colOffset" type="color3" value="0.0, 0.0, 0.0" uiname="Color Offset" uifolder="Color Correct" />
    <input name="colGamma" type="vector3" value="1.0, 1.0, 1.0" uiname="Gamma" uifolder="Color Correct" />

    <!-- Alpha Correct -->
    <input name="alphaGain" type="float" value="1.0" uimin="0.0" uisoftmax="1.0" uiname="Alpha Gain" uifolder="Alpha Correct" />
    <input name="alphaOffset" type="float" value="0.0" uimin="0.0" uisoftmax="1.0" uiname="Alpha Offset" uifolder="Alpha Correct" />
    <input name="alphaGamma" type="float" value="1.0" uimin="0.0" uisoftmax="5.0" uiname="Gamma" uifolder="Alpha Correct" />

    <!-- Clamp Output -->
    <input name="colClamp" type="boolean" value="false" uiname="Clamp Color" uifolder="Clamp Output" />
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/MaterialXGenOgsXml/libraries/maya_utilities.mtlx" line="24">

---

Another example is the `maya_utilities.mtlx` script, which defines utility nodes like `place2dTexture` for UV mapping.

```mtlx
  <nodedef name="MayaND_place2dTexture_vector2" node="place2dTexture" nodegroup="math">
    <input name="uvCoord" type="vector2" value="0.0, 0.0" />
    <input name="coverage" type="vector2" value="1.0, 1.0" />
    <input name="translateFrame" type="vector2" value="0.0, 0.0" />
    <input name="rotateFrame" type="float" value="0.0" unittype="angle" unit="radian" />
    <input name="mirrorU" type="boolean" value="false" />
    <input name="mirrorV" type="boolean" value="false" />
    <input name="wrapU" type="boolean" value="true" />
    <input name="wrapV" type="boolean" value="true" />
    <input name="stagger" type="boolean" value="false" />
    <input name="repeatUV" type="vector2" value="1.0, 1.0" />
    <input name="offset" type="vector2" value="0.0, 0.0" />
    <input name="rotateUV" type="float" value="0.0" unittype="angle" unit="radian" />
    <input name="noiseUV" type="vector2" value="0.0, 0.0" />
    <output name="outUV" type="vector2" />
  </nodedef>
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
