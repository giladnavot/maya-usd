---
title: Getting Started with Chaser
---

Chaser in the maya-usd repository refers to a set of classes and methods that are used to handle specific tasks after the core USD export or import operations. There are two main types of chasers: ImportChaser and ExportChaser. ImportChaser is used after the import operation, it has methods like 'Redo', 'Undo', and 'PostImport'. On the other hand, ExportChaser is used after the export operation, it has methods like 'ExportDefault', 'ExportFrame', and 'PostExport'. These chasers allow for custom processing and modifications after the main import or export operations.

<SwmSnippet path="/lib/mayaUsd/fileio/chaser/exportChaser.h" line="30">

---

## UsdMayaExportChaser

The `UsdMayaExportChaser` class is a base class for plugin chasers that run after the core USD export out of Maya. It provides methods like `ExportDefault`, `ExportFrame`, and `PostExport` that can be overridden to perform custom operations during the export process.

```c
/// \brief base class for plugin chasers which are plugins that run after the
/// core usdExport out of maya.
///
/// Chaser objects will be constructed after the initial "unvarying" export.
/// Chasers should save off necessary data when they are constructed.
/// Afterwards, the chasers will be invoked to export Defaults.  For each frame,
/// after the core process the given frame, all the chasers will be invoked to
/// process that frame.
///
/// The key difference between these and the mel/python postScripts is that a
/// chaser can have direct access to the core usdExport context.
///
/// Chasers need to be very careful as to not modify the structure of the usd
/// file.  This should ideally be used to make small changes or to add
/// attributes in a non-destructive way.
class UsdMayaExportChaser : public TfRefBase
{
public:
    ~UsdMayaExportChaser() override { }

    /// Do custom processing after UsdMaya has exported data at the default
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/chaser/importChaser.h" line="54">

---

## UsdMayaImportChaser

The `UsdMayaImportChaser` class provides a `PostImport` method that can be overridden to perform custom operations after the import process.

```c
    virtual ~UsdMayaImportChaser() override { }

    MAYAUSD_CORE_PUBLIC
    virtual bool PostImport(
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/chaser/importChaserRegistry.cpp" line="80">

---

## Chaser Registries

The `UsdMayaImportChaserRegistry` and `UsdMayaExportChaserRegistry` classes are used to register and create instances of Chasers. The `Create` method is used to create a new instance of a registered Chaser.

```c++
UsdMayaImportChaserRefPtr
UsdMayaImportChaserRegistry::Create(const char* name, const FactoryContext& context) const
{
    TfRegistryManager::GetInstance().SubscribeTo<UsdMayaImportChaserRegistry>();
    if (UsdMayaImportChaserRegistry::FactoryFn fn = _factoryImportRegistry[name]) {
        return TfCreateRefPtr(fn(context));
    } else {
        return TfNullPtr;
    }
}
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
