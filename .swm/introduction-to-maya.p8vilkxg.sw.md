---
title: Introduction to Maya
---

Maya, in the context of the maya-usd repository, refers to the Autodesk Maya software. It's a 3D computer graphics application used for creating interactive 3D applications, including video games, animated films, TV series, and visual effects. The maya-usd repository is designed to provide a Maya plugin and reusable libraries for Pixar's Universal Scene Description (USD). This plugin allows for a more seamless integration of USD workflows into Maya by providing a set of functionalities such as importing and exporting USD files, and also viewing and editing of USD data directly within Maya.

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/__init__.py" line="64">

---

# Maya in the Codebase

The `maya` module is imported in the codebase to interact with Maya's command and scripting language. The `cmds` and `mel` modules from `maya` are used to execute Maya commands and MEL scripts respectively.

```python
from maya import cmds
from maya import mel
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/__init__.py" line="90">

---

# Maya's Main Progress Bar

The `_GetMainProgressBar` function is used to interact with Maya's main progress bar. This is an example of how the codebase interacts with Maya's UI elements.

```python
def _GetMainProgressBar():
    """
    Returns the name of Maya's main progress bar, or None if the main progress
    bar is not available.

    The main progress bar may be unavailable in non-interactive/Python-only
    sessions of Maya.
    """
    whatIsProgressBar = mel.eval('whatIs \"$gMainProgressBar\";')
    if whatIsProgressBar != 'string variable':
        return None

    mainProgressBar = mel.eval('$tmp = $gMainProgressBar')
    return mainProgressBar
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/userExportedAttributesUI.py" line="54">

---

# Maya's User Interface

The `userExportedAttributesUI.py` file contains global constants that define various attributes and options in Maya's user interface. This shows how the codebase customizes and interacts with Maya's UI.

```python
EXPORTED_ATTRS_MAYA_ATTR_NAME = 'USD_UserExportedAttributesJson'

USD_ATTR_TYPE_USD = 'USD'
USD_ATTR_TYPE_PRIMVAR = 'primvar'
USD_ATTR_TYPE_USDRI = 'usdRi'

USD_ATTR_TYPE_OPTIONS = [
    USD_ATTR_TYPE_USD,
    USD_ATTR_TYPE_PRIMVAR,
    USD_ATTR_TYPE_USDRI
]

# The first empty string option here allows the interpolation to be un-specified.
PRIMVAR_INTERPOLATION_OPTIONS = [
    '',
    UsdGeom.Tokens.constant,
    UsdGeom.Tokens.uniform,
    UsdGeom.Tokens.varying,
    UsdGeom.Tokens.vertex,
    UsdGeom.Tokens.faceVarying
]
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/AEpxrUsdReferenceAssemblyTemplate.py" line="94">

---

# Maya's Node Attributes

The `variantSets_Replace_new` function is used to replace node attributes in Maya. This is an example of how the codebase manipulates Maya's node attributes.

```python
# For maya 2022 and above
def variantSets_Replace_new(nodeAttr, new=True):
    variantSets_Replace(nodeAttr, new=True)

def variantSets_Replace_replace(nodeAttr, new=True):
    variantSets_Replace(nodeAttr, new=False)

def variantSets_Replace(nodeAttr, new):
    # Store the original parent and restore it below
    origParent = cmds.setParent(q=True)

    frameLayoutName = 'AEpxrUsdReferenceAssemblyTemplate_variantSets_Layout'
    if new == True:
        cmds.frameLayout(frameLayoutName, label='VariantSets', collapse=False)
    else:
        cmds.setParent(frameLayoutName)

    # Remove existing children of layout
    children = cmds.frameLayout(frameLayoutName, q=True, childArray=True)
    if children:
        for child in children:
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/readJob_ImportWithProxies.cpp" line="292">

---

# Importing with Proxies in Maya

The `_DoImportWithProxies` function is used to import USD prims with proxies in Maya. This is an example of how the codebase uses Maya to handle USD data.

```c++
bool UsdMaya_ReadJobWithSceneAssembly::_DoImportWithProxies(UsdPrimRange& range)
{
    MStatus status;

    // We'll iterate through the prims collecting the various types we're
    // interested in, but we defer creating any Maya nodes until we've finished
    // iterating. This way we'll know all the paths we'll need to re-create in
    // Maya, and we can create only the transforms necessary to produce those
    // paths.
    std::vector<UsdPrim> cameraPrims;
    std::vector<UsdPrim> subAssemblyPrims;
    std::vector<UsdPrim> proxyPrims;

    UsdPrim          pxrGeomRoot;
    CollapsePointMap collapsePointMap;

    for (auto primIt = range.begin(); primIt != range.end(); ++primIt) {
        const UsdPrim& prim = *primIt;

        if (prim.IsA<UsdGeomCamera>()) {
            cameraPrims.push_back(prim);
```

---

</SwmSnippet>

# Key Functions in Maya-USD

This section will cover the functions \_DoImportWithProxies, \_ShouldImportAsSubAssembly, \_IsPxrGeomRoot, and \_CollectCollapsePoints, which are key to the functionality of the Maya-USD plugin.

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/readJob_ImportWithProxies.cpp" line="292">

---

## \_DoImportWithProxies

The `_DoImportWithProxies` function is responsible for importing USD data with proxies. It iterates through the USD prims and collects various types of prims, such as cameras, sub-assemblies, and proxies. It then creates the necessary Maya nodes after finishing the iteration. This function is crucial for the import process of the Maya-USD plugin.

```c++
bool UsdMaya_ReadJobWithSceneAssembly::_DoImportWithProxies(UsdPrimRange& range)
{
    MStatus status;

    // We'll iterate through the prims collecting the various types we're
    // interested in, but we defer creating any Maya nodes until we've finished
    // iterating. This way we'll know all the paths we'll need to re-create in
    // Maya, and we can create only the transforms necessary to produce those
    // paths.
    std::vector<UsdPrim> cameraPrims;
    std::vector<UsdPrim> subAssemblyPrims;
    std::vector<UsdPrim> proxyPrims;

    UsdPrim          pxrGeomRoot;
    CollapsePointMap collapsePointMap;

    for (auto primIt = range.begin(); primIt != range.end(); ++primIt) {
        const UsdPrim& prim = *primIt;

        if (prim.IsA<UsdGeomCamera>()) {
            cameraPrims.push_back(prim);
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/readJob_ImportWithProxies.cpp" line="74">

---

## \_ShouldImportAsSubAssembly

The `_ShouldImportAsSubAssembly` function determines whether a USD prim should be imported as a sub-assembly. It checks the type and kind of the prim to make this decision. This function is important for organizing the imported data in a structured manner.

```c++
static bool _ShouldImportAsSubAssembly(const UsdPrim& prim)
{
    // XXX: We need to identify dressGroups by prim type, since dressGroups
    // nested inside component models will have kind subcomponent rather than
    // dress_group.  We hardcode the typeName to avoid pulling in a pixar-only
    // dependency. Must fix.
    if (prim.GetTypeName() == _tokens->PointInstancerTypeName) {
        return true;
    }

    TfToken     kind;
    UsdModelAPI usdModel(prim);
    usdModel.GetKind(&kind);

    if (KindRegistry::IsA(kind, KindTokens->component)
        || KindRegistry::IsA(kind, KindTokens->assembly)) {
        return true;
    }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/readJob_ImportWithProxies.cpp" line="109">

---

## \_IsPxrGeomRoot

The `_IsPxrGeomRoot` function checks if a USD prim is a 'Geom' scope, which is a top-level proxy node. This function is important for identifying top-level proxy nodes during the import process.

```c++
// XXX: At the moment, the 'Geom' scope is just a convenient place to insert
// a model's top-level proxy. Ultimately, we should always create the top-level
// proxy for the imported model and not depend on the existence of this scope.
static bool _IsPxrGeomRoot(const UsdPrim& prim)
{
    if (prim.GetName() == _tokens->GeomRootName && prim.GetParent() && prim.GetParent().IsModel()) {
        return true;
    }

    return false;
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/readJob_ImportWithProxies.cpp" line="274">

---

## \_CollectCollapsePoints

The `_CollectCollapsePoints` function collects all collapse points below a given USD prim and stores them in a map. This function is crucial for handling complex USD data structures during the import process.

```c++
static void _CollectCollapsePoints(
    const UsdPrim&    prim,
    const UsdPrim&    ancestor,
    CollapsePointMap* collapsePointMap)
{
    for (const UsdPrim& child : prim.GetAllChildren()) {
        if (_IsCollapsePoint(child)) {
            // Cache this collapse point. This node now becomes the exclude
            // ancestor for all child collapse points
            (*collapsePointMap)[ancestor.GetPath()].push_back(child);
            _CollectCollapsePoints(child, child, collapsePointMap);
        } else {
            // Preserve the current exclude ancestor and recurse into children
            _CollectCollapsePoints(child, ancestor, collapsePointMap);
        }
    }
}
```

---

</SwmSnippet>

# Export and Import Commands

Understanding Export and Import Commands in Maya-USD

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/exportCommand.h" line="35">

---

## PxrMayaUSDExportCommand::creator

The `PxrMayaUSDExportCommand::creator` function is a factory method that creates an instance of the export command. This command is used to export a Maya scene or part of it to a USD file.

```c
    static void* creator();
};
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/importCommand.h" line="51">

---

## PxrMayaUSDImportCommand::creator

The `PxrMayaUSDImportCommand::creator` function is a factory method that creates an instance of the import command. This command is used to import a USD file into the Maya scene.

```c
    static void* creator();

```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
