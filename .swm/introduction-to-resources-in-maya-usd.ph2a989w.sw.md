---
title: Introduction to Resources in Maya-USD
---

In the maya-usd repository, 'Resources' refers to a directory that contains various types of files and scripts that are used to support the functionality of the Maya plugin. This includes icons, help tables, scripts, and attribute editor (AE) templates. These resources are used to enhance the user interface, provide help documentation, and define custom controls for the plugin.

<SwmSnippet path="/lib/mayaUsd/resources/scripts/mayaUsdAddMayaReference.mel" line="4">

---

# Scripts

Scripts like `mayaUsdAddMayaReference.mel` are used to perform specific tasks. In this case, it's used to add a Maya reference with a namespace option.

```mel
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

global proc string createRowLayoutforMayaReference(string $parent, string $rowLayoutName, int $nbCols)
{
    // See setDefaultTemplates.mel in Maya.
    global int $gTemplateOptionMenuOffset;
    int $kOffsetText = (`about -mac` ? 8 : 5);
    global int $gTextColumnWidthIndex;
    global int $gSingleWidgetWidthIndex;

    int $cw1 = $gTextColumnWidthIndex;
```

---

</SwmSnippet>

# Attribute Editor Utilities

AE utilities like `ae_template.py` are used to manage and manipulate attributes within the Maya interface. This script, for example, uses a dictionary to store various metadata that a prim contains.

# USD Object Information Retrieval

Function Overview

<SwmSnippet path="/lib/mayaUsd/resources/scripts/mayaUsdUtils.py" line="28">

---

## getPulledInfo Function

The `getPulledInfo` function retrieves various pieces of information about a USD object that has been 'pulled' into the Maya scene. This includes the full DAG path of the object in Maya, the UFE hierarchy item corresponding to the object, the USD path of the object, and the USD prim itself. This function is used to gather information about a USD object that is needed for further operations.

```python
def getPulledInfo(dagPath):
    """
    Retrieves the full DAG path, UFE hierarchy item, FE path of the USD pulled path and the USD prim.
    The USD pulled path and USD prim may be invalid if the pulled object is orphaned.
    """
    # The dagPath must have the pull information that brings us to the USD
    # prim.  If this prim is of type Maya reference, create a specific menu
    # item for it, otherwise return an empty string for the chain of
    # responsibility.

    # The dagPath may come in as a shortest unique name.  Expand it to full
    # name; ls -l would be an alternate implementation.
    sn = om.MSelectionList()
    sn.add(dagPath)
    fullDagPath = sn.getDagPath(0).fullPathName()
    mayaItem = ufe.Hierarchy.createItem(ufe.PathString.path(fullDagPath))
    pathMapper = ufe.PathMappingHandler.pathMappingHandler(mayaItem)
    pulledPath = pathMapper.fromHost(mayaItem.path())
    prim = mayaUsd.ufe.ufePathToPrim(ufe.PathString.string(pulledPath))
    return fullDagPath, mayaItem, pulledPath, prim
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/resources/scripts/mayaUsdUtils.py" line="50">

---

## getDagPathUsdTypeName Function

The `getDagPathUsdTypeName` function retrieves the type name of the USD object that created the node pointed to by the DAG path. This type name is kept in the 'USD_typeName' attribute. This function is used to determine the type of the USD object that corresponds to a given Maya node.

```python
def getDagPathUsdTypeName(dagPath):
    """
    Retrieves the type name of the USD object that created the node pointed to by the DAG path.
    This type name is kept in the 'USD_typeName' attribute.
    Returns None if the attribute is not found.
    """
    sn = om.MSelectionList()
    sn.add(dagPath)
    obj = sn.getDependNode(0)
    node = om.MFnDependencyNode(obj)
    if not node.hasAttribute('USD_typeName'):
        return None

    plug = node.findPlug('USD_typeName', True)
    if not plug:
        return None

    return plug.asString()
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
