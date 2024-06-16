---
title: Exploring the Chaser Concept
---
In the maya-usd repository, 'Chaser' refers to a set of classes and methods that are used to handle specific tasks after the core USD export or import operations. There are two main types of Chasers: ImportChaser and ExportChaser. The ImportChaser is used after the core USD import operation. It provides methods like 'Redo', 'Undo', and 'PostImport'. On the other hand, the ExportChaser is used after the core USD export operation. It provides methods like 'ExportDefault', 'ExportFrame', and 'PostExport'. These Chasers are used to make small changes or add attributes in a non-destructive way to the USD file after the core operations.

<SwmSnippet path="/lib/mayaUsd/fileio/chaser/exportChaser.h" line="30">

---

## UsdMayaExportChaser

The `UsdMayaExportChaser` class is the base class for export chasers. It provides three virtual methods: `ExportDefault`, `ExportFrame`, and `PostExport` that can be overridden to implement the desired functionality. The `ExportDefault` method is called after UsdMaya has exported data at the default time, `ExportFrame` is called after UsdMaya has exported data at a specific time, and `PostExport` is called after the main UsdMaya export loop.

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

<SwmSnippet path="/lib/mayaUsd/fileio/chaser/importChaser.h" line="51">

---

## UsdMayaImportChaser

The `UsdMayaImportChaser` class is the base class for import chasers. It provides a `PostImport` method that can be overridden to implement the desired functionality. The `PostImport` method is called after the main UsdMaya import process.

```c
    MSdfToDagMap sdfToDagMap;

public:
    virtual ~UsdMayaImportChaser() override { }

    MAYAUSD_CORE_PUBLIC
    virtual bool PostImport(
```

---

</SwmSnippet>

<SwmSnippet path="/lib/mayaUsd/fileio/chaser/exportChaserRegistry.h" line="92">

---

## Chaser Registries

The `UsdMayaExportChaserRegistry` and `UsdMayaImportChaserRegistry` classes are used to register and create instances of chasers. They provide a `Create` method to create instances of the registered chasers and a `GetAllRegisteredChasers` method to get a list of all registered chasers.

```c
    typedef std::function<UsdMayaExportChaser*(const FactoryContext&)> FactoryFn;

    /// \brief Register a chaser factory.
    ///
    /// Please use the \p PXRUSDMAYA_DEFINE_CHASER_FACTORY instead of calling
    /// this directly.
    MAYAUSD_CORE_PUBLIC
    bool RegisterFactory(const std::string& name, FactoryFn fn, bool fromPython = false);

    /// \brief Creates a chaser using the factoring registered to \p name.
    MAYAUSD_CORE_PUBLIC
    UsdMayaExportChaserRefPtr Create(const std::string& name, const FactoryContext& context) const;

    /// \brief Returns the names of all registered chasers.
    MAYAUSD_CORE_PUBLIC
    std::vector<std::string> GetAllRegisteredChasers() const;

    MAYAUSD_CORE_PUBLIC
    static UsdMayaExportChaserRegistry& GetInstance();

private:
```

---

</SwmSnippet>

&nbsp;

*This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human*

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd" doc-type="overview"><sup>Powered by [Swimm](/)</sup></SwmMeta>
