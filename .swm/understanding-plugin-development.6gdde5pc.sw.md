---
title: Understanding Plugin Development
---

Plugin Development in the maya-usd repository involves creating reusable libraries and functionalities for Pixar's Universal Scene Description (USD). This is done within the 'plugin' directory, specifically within the 'pxr/maya/lib/pxrUsdMayaGL' subdirectory. Here, you can find various test environments for different aspects of the plugin, such as 'BatchRendererIsolateSelectionTest', 'InstancerDrawTest', and 'RefAssemblyDrawRepresentationsTest'. These tests are crucial for ensuring the correct functionality of the developed plugins.

<SwmSnippet path="/plugin/pxr/maya/lib/pxrUsdMayaGL/testenv/testPxrUsdMayaGLInstancerDraw.py" line="6">

---

# Test Files in Plugin Development

This file is an example of a test file in plugin development. It's used to test the 'pxrUsdMayaGLInstancerDraw' plugin. The tests are run in a specific workspace, as indicated on line 46. The test on line 133 is for the 'Cards' drawmode of the assembly.

```python
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
from maya import cmds

import os
import sys
import unittest

import fixturesUtils
import imageUtils
import mayaUtils

```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/pxrUsdMayaGL/testenv/testBatchRendererIsolateSelection.py" line="6">

---

# Selection Testing in Plugin Development

This file is another example of a test file in plugin development. It's used to test the 'BatchRendererIsolateSelection' feature of the plugin. The comment on line 97 indicates that the test is for the selection hit-testing functionality.

```python
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from pxr import UsdMaya

from maya import cmds

import os
import sys
import unittest

import fixturesUtils
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/pxrUsdMayaGL/testenv/testRefAssemblyDrawRepresentations.py" line="6">

---

# Workspace in Plugin Development

This file is a test file for the 'RefAssemblyDrawRepresentations' feature of the plugin. The comment on line 58 indicates that the tests are run in a specific workspace.

```python
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from pxr import UsdMaya

from maya import cmds

import os
import sys
import unittest

import fixturesUtils
```

---

</SwmSnippet>

# Plugin Development Functions

This section discusses the functions involved in plugin development.

<SwmSnippet path="/plugin/adsk/plugin/plugin.cpp" line="1">

---

## Plugin Development Functions

This file contains functions that are crucial for the development of the Autodesk plugin. These functions handle the initialization and registration of the plugin with Maya.

```c++
//
// Copyright 2019 Autodesk
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
#include "ProxyShape.h"
#include "ProxyShapeListener.h"
#include "adskExportCommand.h"
#include "adskImportCommand.h"
#include "adskListJobContextsCommand.h"
#include "adskListShadingModesCommand.h"
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/plugin/pxrUsdPreviewSurface/plugin.cpp" line="1">

---

This file contains functions that are essential for the development of the Pixar USD Preview Surface plugin. These functions manage the initialization and registration of the plugin with Maya.

```c++
//
// Copyright 2018 Pixar
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
#include "api.h"

#include <mayaUsd/fileio/shaderReaderRegistry.h>
#include <mayaUsd/fileio/shaderWriterRegistry.h>

#include <maya/MFnPlugin.h>
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
