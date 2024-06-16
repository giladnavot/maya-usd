---
title: Understanding Job Operations
---

In the context of the maya-usd repository, 'Jobs' refer to operations that are performed to read from or write to USD files. These operations are encapsulated in classes like 'UsdMaya_ReadJob' and 'UsdMaya_WriteJob'. The 'UsdMaya_ReadJob' class is responsible for reading a USD stage and creating corresponding Maya nodes. It provides methods to perform, undo, and redo the read operation. The 'UsdMaya_WriteJob' class, on the other hand, is responsible for writing a Maya scene to a USD file. It provides a method to perform the write operation.

<SwmSnippet path="/lib/mayaUsd/fileio/jobs/jobArgs.h" line="36">

---

# Job Arguments

The `jobArgs.h` file defines the arguments that can be passed to a job. These arguments control various aspects of the import/export process, such as what data to convert, how to convert it, and where to store the converted data.

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

The `writeJob.h` file defines the `UsdMaya_WriteJob` class, which is used to export data from Maya to USD. An instance of this class is created with the appropriate job arguments, and then the `Write` method is called to perform the export operation.

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

# Job Endpoints

Overview of Job Endpoints

<SwmSnippet path="/lib/mayaUsd/fileio/jobs/writeJob.h" line="50">

---

## UsdMaya_WriteJob::Write

The `Write` method of the `UsdMaya_WriteJob` class is responsible for writing the Maya stage to a USD file. It takes a file name and a boolean flag indicating whether to append to an existing stage or replace it. The method returns a boolean indicating the success of the operation.

```c
    bool Write(const std::string& fileName, bool append);
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/jobs/modelKindProcessor.cpp" line="94">

---

## UsdMaya_ModelKindProcessor::MakeModelHierarchy

The `MakeModelHierarchy` method of the `UsdMaya_ModelKindProcessor` class is responsible for creating a valid model hierarchy on the given USD stage. It returns a boolean indicating the success of the operation.

```c++
bool UsdMaya_ModelKindProcessor::MakeModelHierarchy(UsdStageRefPtr& stage)
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
