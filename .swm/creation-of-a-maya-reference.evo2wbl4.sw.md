---
title: Creation of a Maya Reference
---
This document will cover the process of creating a Maya reference in the Maya-USD plugin, which includes:

1. The creation of the Maya reference
2. Finding an existing Maya reference
3. Connecting the reference to an associated node
4. Executing the export command
5. Getting the exporter
6. Ending the process

```mermaid
graph TD;
subgraph lib/mayaUsd/fileio
  update:::mainFlowStyle --> CreateMayaReference
end
subgraph lib/mayaUsd/fileio
  update:::mainFlowStyle --> findExistingMayaReference
end
subgraph lib/mayaUsd/fileio
  findExistingMayaReference:::mainFlowStyle --> connectReferenceAssociatedNode
end
subgraph lib/mayaUsd/commands/baseExportCommand.cpp
  connectReferenceAssociatedNode:::mainFlowStyle --> doIt
end
subgraph lib/mayaUsd/fileio/shading
  doIt:::mainFlowStyle --> GetExporter
end
subgraph lib/mayaUsd/fileio/shading
  GetExporter:::mainFlowStyle --> _GetExporter
end
subgraph lib/mayaUsd/fileio
  _GetExporter:::mainFlowStyle --> end
end
  end:::mainFlowStyle --> ...

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorMayaReference.cpp" line="294">

---

# Creating the Maya Reference

The function `CreateMayaReference` is used to create a new Maya reference. It takes in parameters such as the UsdPrim, parent object, maya reference path, and namespace. It then constructs a command to create a new reference node in Maya, executes the command, and checks for any errors. If successful, it connects the prim transform's message to the reference's `associatedNode` attribute, sets a custom attribute, and loads the reference.

```c++
MStatus UsdMayaTranslatorMayaReference::CreateMayaReference(
    const UsdPrim& prim,
    MObject&       parent,
    MString&       mayaReferencePath,
    MString&       rigNamespaceM,
    bool           mergeNamespacesOnClash)
{
    TF_DEBUG(PXRUSDMAYA_TRANSLATORS)
        .Msg("MayaReferenceLogic::CreateMayaReference prim=%s\n", prim.GetPath().GetText());
    MStatus status;

    MFnDagNode parentDag(parent, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // Need to create new reference (initially unloaded).
    //
    // When we create reference nodes, we want a separate reference node to be
    // created for each proxy, even proxies that are duplicates of each other.
    // This is to ensure that edits to each copy of an asset are preserved
    // separately.  To this end, we must create a unique name for each proxy's
    // reference node.  Simply including namespace information (if any) from the
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/translators/translatorMayaReference.cpp" line="413">

---

# Finding an Existing Maya Reference

The function `connectReferenceAssociatedNode` is used to connect the reference to an associated node. It retrieves the `message` attribute from the parent node and the `associatedNode` attribute from the reference node. If they are not already connected, it creates a connection between them.

```c++
static MStatus connectReferenceAssociatedNode(MFnDagNode& dagNode, MFnReference& refNode)
{
    MPlug srcPlug(dagNode.object(), getMessageAttr());
    /*
       From the Maya docs:
       > This message attribute is used to connect specific nodes that may be
       > associated with this reference (i.e. group, locator, annotation). Use of
       > this connection indicates that the associated nodes have the same
       > lifespan as the reference, and will be deleted along with the reference
       > if it is removed.
     */
    MStatus      result;
    MPlug        destArrayPlug(refNode.object(), getAssociatedNodeAttr());
    bool         wasConnected = false;
    unsigned int destIndex = 0;
    result = connectedOrFirstAvailableIndex(srcPlug, destArrayPlug, destIndex, wasConnected);
    CHECK_MSTATUS_AND_RETURN_IT(result);
    if (wasConnected) {
        // If it's already connected, abort, we're done
        return result;
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/commands/baseExportCommand.cpp" line="288">

---

# Executing the Export Command

The `doIt` function is used to execute the export command. It first checks if all flags were valid, then reads all of the dictionary args and other args specific to this command. It then initializes a write job with these arguments and writes to the file.

```c++
MStatus MayaUSDExportCommand::doIt(const MArgList& args)
{
    try {
        MStatus status;

        MArgDatabase argData(syntax(), args, &status);

        // Check that all flags were valid
        if (status != MS::kSuccess) {
            return status;
        }

        if (argData.isFlagSet("shadingMode")) {
            MString stringVal;
            argData.getFlagArgument("shadingMode", 0, stringVal);
            TfToken shadingMode(stringVal.asChar());

            if (!shadingMode.IsEmpty()
                && UsdMayaShadingModeRegistry::GetInstance().GetExporter(shadingMode) == nullptr
                && shadingMode != UsdMayaShadingModeTokens->none) {
                MGlobal::displayError(
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/shading/shadingModeRegistry.h" line="75">

---

# Getting the Exporter

The function `GetExporter` is used to get the exporter for a given shading mode. It calls the `_GetExporter` function to do this.

```c
    static UsdMayaShadingModeExporterCreator GetExporter(const TfToken& name)
    {
        return GetInstance()._GetExporter(name);
    }
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/primUpdaterManager.cpp" line="847">

---

# Ending the Process

The `end` function is used to end the process. It checks if there is a controlling flag and if so, sets it to false and nulls it out.

```c++
    void end()
    {
        if (_controllingFlag) {
            *_controllingFlag = false;
            _controllingFlag = nullptr;
        }
    }
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="flows"><sup>Powered by [Swimm](/)</sup></SwmMeta>
