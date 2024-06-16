---
title: Overview of usdMaya
---

usdMaya is a crucial part of the maya-usd repository. It is a Maya plugin and reusable libraries for Pixar's Universal Scene Description (USD). It is used to create a bridge between Maya and USD, allowing for the translation of data between the two. This includes the ability to import and export data, as well as the ability to edit and manipulate data within Maya. It is used extensively throughout the codebase, with various functions and classes that handle different aspects of the translation process.

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/usdMaya.mel" line="1">

---

# usdMaya Procedures

This is an example of a usdMaya procedure, `usdMaya_CreateUsdProxyShape`, which is used to create a USD proxy shape in Maya. It prompts the user to select a USD file, creates a new USD proxy shape node, and sets the selected file as the node's file path.

```mel
global proc usdMaya_CreateUsdProxyShape()
{
    string $fileName = `fileDialog -mode 0 -dm "*.usd *.usda *.usdc" -t "Choose a USD File"`;
    if (size(strip($fileName)) == 0) {
        return;
    }

    string $baseName = capitalizeString(`basenameEx $fileName`);
    if( isValidObjectName($baseName) )
        $baseName += "_usd";
    else
        $baseName = "UsdStage";

    string $shapeNode = `createNode "pxrUsdProxyShape" -skipSelect -name ($baseName+"Shape")`;
    setAttr -type "string" ($shapeNode+".filePath") $fileName;

    select -r $shapeNode;
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/usdMaya.mel" line="20">

---

This is another usdMaya procedure, `usdMaya_SetAssemblyVariant`, which is used to set a variant on a USD assembly. It takes a node shape, a variant set name, and a variant as arguments, and sets the specified variant on the variant set of the node shape.

```mel
global proc int usdMaya_SetAssemblyVariant(string $nodeShape, string $variantSetName, string $variant)
{
    //
    // Reference: AEUsdAssemblyTemplate.mel -> AEUsdAssemblyTemplate_variantSets_Replace()
    //
    int $success = false;
    int $foundVariantSet = false;
    string $nodeAttr = ($nodeShape+".variantSets");
    int $indices[] = `getAttr -mi $nodeAttr`;
    int $index;
    for ($index in $indices) {
        string $iterVariantSet  = `getAttr ($nodeAttr+"["+$index+"].variantSet")`;
        if ($iterVariantSet != $variantSetName) {
            continue;
        }
        $foundVariantSet = true;
        string $variantAttr = ($nodeAttr+"["+$index+"].variant");
        if (`getAttr $variantAttr` != $variant) {
            if (`getAttr -settable $variantAttr`) {
                setAttr -type "string" $variantAttr $variant;
                $success = true;
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/usdMaya/editUtil.h" line="39">

---

# usdMaya Utilities

This is an example of a usdMaya utility, `UsdMayaEditUtil`, which is used to handle edits on assemblies in Maya. It provides a set of methods for inspecting and applying edits, such as `GetEditFromString`, `GetEditsForAssembly`, and `ApplyEditsToProxy`.

```c
class UsdMayaEditUtil
{
public:
    /// \name Enums for inspecting edits
    /// \{

    /// Possible operations for a supported edit.
    enum EditOp
    {
        OP_TRANSLATE,
        OP_ROTATE,
        OP_SCALE
    };

    /// Whether the edit affects one component or all components.
    /// The values are explicit, such that X,Y,and Z can be used in []
    /// operators on Vec3s.
    ///
    enum EditSet
    {
        SET_ALL = -1,
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
