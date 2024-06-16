---
title: Batching Small Changes in a Transaction
---

This document will cover the process of batching small changes together in a transaction within the Maya-USD plugin. The main points of discussion are:

1. The role of the Transaction class
2. The functionality of the TransactionManager class
3. The use of the ScopedTransaction class

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/Transaction.h" line="28">

---

# The Role of the Transaction Class

The Transaction class provides an interface for opening and closing transactions. It is responsible for managing the transaction logic. The class has methods for opening and closing transactions, and checking if a transaction is in progress.

```c
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

    /// \brief  opens transaction, when transaction is opened for the first time OpenNotice is
    /// emitted and current
    ///         state of layer is recorded.
    /// \note   It's valid to call Open multiple times, but they need to balance Close calls
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/TransactionManager.h" line="30">

---

# The Functionality of the TransactionManager Class

The TransactionManager class provides an interface for managing transactions targeting any stage and layer. It is used by the Transaction class to avoid stage lookup.

```c
///         transactions targeting any stage and layer.
///         It provides both direct interface used by Transaction class, which avoids stage lookup,
///         as well as static interface where stage needs to be provided.
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/Transaction.h" line="73">

---

# The Use of the ScopedTransaction Class

The ScopedTransaction class is a helper class that binds a transaction session to the lifetime of a class instance. A transaction is opened when a ScopedTransaction is constructed and closed when it is destructed.

```c
/// \brief  This is a helper class that binds transaction session to lifetime of class instance.
///         Transaction is opened when ScopeTransaction is constructed and closed when destructed.
//----------------------------------------------------------------------------------------------------------------------
class ScopedTransaction
{
public:
    /// \brief  the ctor initializes transaction and opens it
    /// \param  stage that will be notified about transaction open/close
    /// \param  layer that will be tracked for changes
    inline ScopedTransaction(
        const PXR_NS::UsdStageWeakPtr& stage,
        const PXR_NS::SdfLayerHandle&  layer)
        : m_transaction(stage, layer)
    {
        m_transaction.Open();
    }

    /// \brief  the dtor closes transaction
    inline ~ScopedTransaction() { m_transaction.Close(); }

private:
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
