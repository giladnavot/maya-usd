---
title: Basic Concepts of Transaction in Maya-USD
---

The Transaction class in the maya-usd repository provides an interface for opening and closing transactions. It is defined for a given stage and layer. When a transaction is opened, the current state of the layer is registered and will be compared with the state upon transaction close. It's the user's responsibility to pair Open with Close calls. The Transaction class also provides information about whether a transaction was opened and hasn't been closed yet.

The Transaction class is used in conjunction with the TransactionManager class. The TransactionManager class manages transactions targeting any stage and layer. It provides both a direct interface used by the Transaction class, which avoids stage lookup, as well as a static interface where the stage needs to be provided.

There is also a helper class called ScopedTransaction that binds a transaction session to the lifetime of a class instance. A transaction is opened when a ScopedTransaction is constructed and closed when it is destructed. This makes it easier to manage the opening and closing of transactions.

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/Transaction.h" line="36">

---

# Transaction Class

This is the definition of the Transaction class. It includes methods for opening and closing transactions, as well as checking if a transaction is in progress. The constructor takes a stage and a layer as parameters, which are the elements that will be affected by the transaction.

```c
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
    /// \return true on success, false when layer or stage became invalid
    AL_USD_TRANSACTION_PUBLIC
    bool Open() const;

    /// \brief  closes transaction, when transaction is closed for the last time CloseNotice is
    /// emitted with change
    ///         information based of difference between current and recorded layer states.
    /// \note   It's valid to call Close multiple times, but they need to balance Open calls
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/wrapTransaction.cpp" line="31">

---

# Using the Transaction Class

This is an example of how to use the Transaction class. An instance of the class is created with a stage and layer as parameters. The Open method is then called to start the transaction, and the Close method is called to end it.

```c++
        typedef AL::usd::transaction::Transaction This;
        class_<This>("Transaction", no_init)
            .def(init<const UsdStageWeakPtr&, const SdfLayerHandle&>((arg("stage"), arg("layer"))))
            .def("Open", &This::Open)
            .def("Close", &This::Close)
            .def("InProgress", &This::InProgress);
```

---

</SwmSnippet>

# Transaction Management

This section will cover the main functions related to transactions in the 'maya-usd' repository.

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/Transaction.h" line="36">

---

## Transaction Class

The 'Transaction' class provides the interface for opening and closing transactions. It has methods like 'Open', 'Close', and 'InProgress' which respectively open a transaction, close a transaction, and check if a transaction is in progress. The 'Transaction' class uses a 'TransactionManager' to manage the transactions.

```c
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
    /// \return true on success, false when layer or stage became invalid
    AL_USD_TRANSACTION_PUBLIC
    bool Open() const;

    /// \brief  closes transaction, when transaction is closed for the last time CloseNotice is
    /// emitted with change
    ///         information based of difference between current and recorded layer states.
    /// \note   It's valid to call Close multiple times, but they need to balance Open calls
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/TransactionManager.h" line="43">

---

## TransactionManager Class

The 'TransactionManager' class manages multiple transactions targeting any stage and layer. It provides methods like 'Open', 'Close', 'InProgress', and 'AnyInProgress' which respectively open a transaction, close a transaction, check if a specific transaction is in progress, and check if any transaction is in progress. The 'TransactionManager' class uses a 'TransactionData' structure to store the state of each transaction.

```c
class TransactionManager
{
public:
    /// \brief  provides information whether transaction was opened and wasn't closed yet.
    /// \param  layer targetted by transaction
    /// \return true when transaction is in progress, otherwise false
    AL_USD_TRANSACTION_PUBLIC
    bool InProgress(const PXR_NS::SdfLayerHandle& layer) const;

    /// \brief  opens transaction, when transaction is opened for the first time OpenNotice is
    /// emitted and current
    ///         state of layer is recorded.
    /// \note   It's valid to call Open multiple times, but they need to balance Close calls
    /// \param  layer targetted by transaction
    /// \return true on success, false when layer or stage became invalid
    AL_USD_TRANSACTION_PUBLIC
    bool Open(const PXR_NS::SdfLayerHandle& layer);

    /// \brief  closes transaction, when transaction is closed for the last time CloseNotice is
    /// emitted with change
    ///         information based of difference between current and recorded layer states.
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/Transaction.h" line="76">

---

## ScopedTransaction Class

The 'ScopedTransaction' class is a helper class that binds a transaction session to the lifetime of a class instance. A transaction is opened when a 'ScopedTransaction' is constructed and closed when it is destructed. This class uses a 'Transaction' object to manage the transaction.

```c
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
    ScopedTransaction() = delete;
    ScopedTransaction(const ScopedTransaction&) = delete;
    ScopedTransaction& operator=(const ScopedTransaction&) = delete;
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
