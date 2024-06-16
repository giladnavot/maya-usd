---
title: Understanding Job Operations
---
In the context of the maya-usd repository, 'Jobs' refer to operations related to reading, writing, and managing data between Maya and USD. They are encapsulated in classes like 'UsdMaya_ReadJob', 'UsdMaya_WriteJob', and their arguments are managed by 'UsdMayaJobExportArgs' and 'UsdMayaJobImportArgs'.&nbsp;

These jobs handle the conversion of data between Maya and USD, ensuring that the data is correctly translated and preserved. They also provide functionalities for undoing and redoing operations, which is crucial for maintaining the integrity of the data during the conversion process.

<SwmSnippet path="/lib/mayaUsd/fileio/jobs/jobArgs.h" line="36">

---

# Job Arguments

This file defines the arguments used for import and export jobs. These arguments are encapsulated in the `UsdMayaJobExportArgs` and `UsdMayaJobImportArgs` classes. They include various settings and options that control how the import or export operation is performed.

```c
PXR_NAMESPACE_OPEN_SCOPE

// clang-format off
#define PXRUSDMAYA_TRANSLATOR_TOKENS \
    ((UsdFileExtensionDefault, "usd")) \
    ((UsdFileExtensionASCII, "usda")) \
    ((UsdFileExtensionCrate, "usdc")) \
    ((UsdFileExtensionPackage, "usdz")) \
    ((UsdReadableFileFilter, "*.usd *.usda *.usdc *.usdz")) \
    ((UsdWritableFileFilter, "*.usd *.usda *.usdc *.usdz"))
// clang-format on

TF_DECLARE_PUBLIC_TOKENS(
    UsdMayaTranslatorTokens,
    MAYAUSD_CORE_PUBLIC,
    PXRUSDMAYA_TRANSLATOR_TOKENS);

// clang-format off
#define PXRUSDMAYA_JOB_EXPORT_ARGS_TOKENS \
    /* Dictionary keys */ \
    (animation) \
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/jobs/writeJob.h" line="31">

---

# Write Job

This file defines the `UsdMaya_WriteJob` class, which is used to perform export operations. It uses an instance of `UsdMayaJobExportArgs` to control the operation. The `Write` method is used to perform the actual export.

```c
PXR_NAMESPACE_OPEN_SCOPE

class UsdMaya_ModelKindProcessor;

class UsdMaya_WriteJob
{
public:
    MAYAUSD_CORE_PUBLIC
    UsdMaya_WriteJob(const UsdMayaJobExportArgs& iArgs);

    MAYAUSD_CORE_PUBLIC
    ~UsdMaya_WriteJob();

    /// Writes the Maya stage to the given USD file name, If \p append is
    /// \c true, adds to an existing stage. Otherwise, replaces any existing
    /// file.
    /// This will write the entire frame range specified by the export args.
    /// Returns \c true if successful, or \c false if an error was encountered.
    MAYAUSD_CORE_PUBLIC
    bool Write(const std::string& fileName, bool append);

```

---

</SwmSnippet>

# Job Execution Methods

Job Execution in Maya-USD

<SwmSnippet path="/lib/mayaUsd/fileio/jobs/writeJob.h" line="44">

---

## UsdMaya_WriteJob::Write

The `Write` method of the `UsdMaya_WriteJob` class is used to write the Maya stage to a given USD file. It takes a file name and a boolean flag indicating whether to append to an existing stage or replace any existing file. It returns a boolean indicating the success of the operation.

```c
    /// Writes the Maya stage to the given USD file name, If \p append is
    /// \c true, adds to an existing stage. Otherwise, replaces any existing
    /// file.
    /// This will write the entire frame range specified by the export args.
    /// Returns \c true if successful, or \c false if an error was encountered.
    MAYAUSD_CORE_PUBLIC
    bool Write(const std::string& fileName, bool append);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/jobs/modelKindProcessor.cpp" line="94">

---

## UsdMaya_ModelKindProcessor::MakeModelHierarchy

The `MakeModelHierarchy` method of the `UsdMaya_ModelKindProcessor` class is used to create a valid model hierarchy on the USD stage. It returns a boolean indicating the success of the operation.

```c++
bool UsdMaya_ModelKindProcessor::MakeModelHierarchy(UsdStageRefPtr& stage)
{
    // For any root-prim that doesn't already have an authored kind
    // (thinking ahead to being able to specify USD_kind per bug/128430),
    // make it a model.  If there were any gprims authored directly during
    // export, we will make the roots be component models, and author
    // kind=subcomponent on any prim-references that would otherwise
    // evaluate to some model-kind; we may in future make this behavior
    // a jobargs option.
    //
    // If there were no gprims directly authored, we'll make it an assembly
    // instead, and attempt to create a valid model-hierarchy if any of the
    // references we authored are references to models.
    //
    // Note that the code below does its best to facilitate having multiple,
    // independent root-trees/models in the same export, however the
    // analysis we have done about gprims and references authored is global,
    // so all trees will get the same treatment/kind.

    if (_args.disableModelKindProcessor) {
        return true;
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](https://staging.swimm.cloud/)</sup></SwmMeta>
