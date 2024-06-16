---
title: Fundamentals of Rendering in Maya-USD
---

Rendering in the maya-usd repository refers to the process of generating an image from a 3D scene. This is done by converting the scene data, which includes geometry, lighting, shading, and camera information, into a 2D image. The rendering process is crucial in visualizing the scene created in Maya with the help of the maya-usd plugin.

<SwmSnippet path="/lib/mayaUsd/render/vp2RenderDelegate/proxyRenderDelegate.cpp" line="1">

---

# Rendering Process

This file contains the implementation of the ProxyRenderDelegate class, which is responsible for the rendering process in the viewport. It uses the Hydra render delegate to perform the rendering.

```c++
//
// Copyright 2020 Autodesk
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "proxyRenderDelegate.h"

#include "drawItem.h"
#include "material.h"
#include "mayaPrimCommon.h"
#include "renderDelegate.h"
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/render/mayaToHydra/renderGlobals.cpp" line="1">

---

# Renderer Selection

This file contains the implementation of the RenderGlobals class, which is responsible for managing the render settings. It includes the selection of the renderer, which can be Arnold, V-Ray, or Renderman.

```c++
//
// Copyright 2019 Luma Pictures
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "renderGlobals.h"

#include "pxr/usd/usdRender/settings.h"
#include "renderOverride.h"
#include "utils.h"

```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
