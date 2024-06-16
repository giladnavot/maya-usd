---
title: Introduction to Usd Transaction
---

Usd Transaction in the maya-usd repository refers to a functionality that allows for the management of changes to a USD stage. It is encapsulated within the `AL::usd::transaction` namespace. The `Transaction` class, defined in `Transaction.h` and implemented in `Transaction.cpp`, is the primary interface for this functionality. It provides methods for initiating, committing, and reverting changes to a USD stage. The `ScopedTransaction` class, also defined in `Transaction.h`, provides a way to automatically manage the lifecycle of a `Transaction` instance. The `OpenNotice` function in `Notice.h` is used to track the changes made during a transaction.

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/Transaction.h" line="24">

---

# Transaction Class

The Transaction class is defined within the transaction namespace. It contains a reference to a TransactionManager and a SdfLayerHandle. The TransactionManager is used to manage the transaction, while the SdfLayerHandle represents the layer of the USD stage that the transaction is operating on.

```c
namespace transaction {

class TransactionManager;

//----------------------------------------------------------------------------------------------------------------------
/// \brief  This is a transaction class which provides interface for opening and closing
/// transactions.
///         Management of transaction logic is performed by Manager class.
///         It's user responsibilty to pair Open with Close calls, otherwise clients might not
///         respond to any further changes. As such ScopedTransaction should be preferred whenever
///         possible.
//----------------------------------------------------------------------------------------------------------------------
class Transaction
{
public:
    /// \brief  the ctor retrieves manager for given stage and sets layer for tracking
    /// \param  stage that will be notified about transaction open/close
    /// \param  layer that will be tracked for changes
    AL_USD_TRANSACTION_PUBLIC
    Transaction(const PXR_NS::UsdStageWeakPtr& stage, const PXR_NS::SdfLayerHandle& layer);

```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/Transaction.cpp" line="23">

---

# Transaction Constructor

The Transaction constructor takes a UsdStageWeakPtr and a SdfLayerHandle as arguments. The UsdStageWeakPtr represents the USD stage that the transaction will operate on, while the SdfLayerHandle represents the layer of the stage that the transaction will operate on.

```c++
namespace AL {
namespace usd {
namespace transaction {

Transaction::Transaction(const UsdStageWeakPtr& stage, const SdfLayerHandle& layer)
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/Transaction.h" line="95">

---

# ScopedTransaction Class

The ScopedTransaction class is a helper class that automatically begins a transaction when it is created and commits the transaction when it is destroyed. This ensures that all operations performed within the scope of the ScopedTransaction object are part of the same transaction.

```c
    ScopedTransaction(const ScopedTransaction&) = delete;
    ScopedTransaction& operator=(const ScopedTransaction&) = delete;
    Transaction        m_transaction;
};
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
