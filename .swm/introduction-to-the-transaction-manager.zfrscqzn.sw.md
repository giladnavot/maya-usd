---
title: Introduction to the Transaction Manager
---

The Transaction Manager in the maya-usd repository is a class that manages transactions targeting any stage and layer. It provides an interface for opening and closing multiple transactions. When a new transaction is opened, an OpenNotice is emitted and a snapshot of the given layer is taken. When the last transaction targeting a given layer for a given stage is closed, the targeted layer content is compared against the previously taken snapshot and a CloseNotice is emitted with delta information. It's important to pair Open with Close calls, otherwise clients might not respond to any further changes.

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/TransactionManager.h" line="43">

---

# TransactionManager Class

This is the definition of the TransactionManager class. It provides methods for managing transactions, such as 'Open', 'Close', and 'InProgress'. It also has a static interface where the stage needs to be provided.

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

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/TransactionManager.cpp" line="98">

---

# Using TransactionManager

Here is an example of how the TransactionManager class is used in the code. The 'InProgress', 'Open', and 'Close' methods are used to manage transactions.

```c++
TransactionManager::StageManagerMap& TransactionManager::GetManagers()
{
    static StageManagerMap managers;
    return managers;
}

//----------------------------------------------------------------------------------------------------------------------
bool TransactionManager::InProgress(const SdfLayerHandle& layer) const
{
    return m_transactions.find(get_pointer(layer)) != m_transactions.end();
}

//----------------------------------------------------------------------------------------------------------------------
bool TransactionManager::AnyInProgress() const { return !m_transactions.empty(); }

//----------------------------------------------------------------------------------------------------------------------
bool TransactionManager::Open(const SdfLayerHandle& layer)
{
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/wrapTransactionManager.cpp" line="28">

---

# TransactionManager in Other Files

The TransactionManager class is also used in other files in the project. For example, in the 'wrapTransactionManager.cpp' file, the TransactionManager class is wrapped for use in Python.

```c++
typedef AL::usd::transaction::TransactionManager This;

static bool InProgressStage(const UsdStageWeakPtr& stage) { return This::InProgress(stage); }

static bool InProgressStageLayer(const UsdStageWeakPtr& stage, const SdfLayerHandle& layer)
{
    return This::InProgress(stage, layer);
}

static bool OpenStageLayer(const UsdStageWeakPtr& stage, const SdfLayerHandle& layer)
{
    return This::Open(stage, layer);
}

static bool CloseStageLayer(const UsdStageWeakPtr& stage, const SdfLayerHandle& layer)
{
    return This::Close(stage, layer);
}

static void CloseAllStage() { This::CloseAll(); }

```

---

</SwmSnippet>

# Transaction Manager Functions

Let's delve into the main functions of the Transaction Manager.

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/TransactionManager.cpp" line="97">

---

## GetManagers

The `GetManagers` function is a static function that returns a reference to the map of Transaction Managers. Each Transaction Manager is associated with a USD Stage. This function is used to retrieve the Transaction Manager for a specific stage.

```c++
//----------------------------------------------------------------------------------------------------------------------
TransactionManager::StageManagerMap& TransactionManager::GetManagers()
{
    static StageManagerMap managers;
    return managers;
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/TransactionManager.cpp" line="104">

---

## InProgress

The `InProgress` function checks if a transaction is in progress for a given stage or layer. It returns true if a transaction is in progress, and false otherwise. This function is crucial for clients to know whether they should respond to updates or not.

```c++
//----------------------------------------------------------------------------------------------------------------------
bool TransactionManager::InProgress(const SdfLayerHandle& layer) const
{
    return m_transactions.find(get_pointer(layer)) != m_transactions.end();
}

//----------------------------------------------------------------------------------------------------------------------
bool TransactionManager::AnyInProgress() const { return !m_transactions.empty(); }

//----------------------------------------------------------------------------------------------------------------------
bool TransactionManager::Open(const SdfLayerHandle& layer)
{
    if (m_stage && layer) {
        auto pair = m_transactions.emplace(get_pointer(layer), TransactionData { nullptr, 1 });
        if (pair.second) {
            auto& base = pair.first->second.base;
            base = SdfLayer::CreateAnonymous("transaction_base");
            base->TransferContent(layer);
            OpenNotice(layer).Send(m_stage);
        } else {
            ++pair.first->second.count;
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/TransactionManager.cpp" line="113">

---

## Open

The `Open` function opens a transaction for a given layer. When a transaction is opened for the first time, an OpenNotice is emitted and the current state of the layer is recorded. This function returns true on success, and false when the layer or stage is invalid.

```c++
//----------------------------------------------------------------------------------------------------------------------
bool TransactionManager::Open(const SdfLayerHandle& layer)
{
    if (m_stage && layer) {
        auto pair = m_transactions.emplace(get_pointer(layer), TransactionData { nullptr, 1 });
        if (pair.second) {
            auto& base = pair.first->second.base;
            base = SdfLayer::CreateAnonymous("transaction_base");
            base->TransferContent(layer);
            OpenNotice(layer).Send(m_stage);
        } else {
            ++pair.first->second.count;
        }
        return true;
    }
    TF_WARN("Provided layer is invalid");
    return false;
}
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/TransactionManager.cpp" line="132">

---

## Close

The `Close` function closes a transaction for a given layer. When a transaction is closed for the last time, a CloseNotice is emitted with change information based on the difference between the current and recorded layer states. This function returns true on success, and false when the layer or stage is invalid or the transaction wasn't opened.

```c++
//----------------------------------------------------------------------------------------------------------------------
bool TransactionManager::Close(const SdfLayerHandle& layer)
{
    if (m_stage && layer) {
        auto it = m_transactions.find(get_pointer(layer));
        if (it != m_transactions.end()) {
            if (--it->second.count == 0) {
                SdfPathVector changedInfo, resynched;
                comparePrims(
                    it->second.base->GetPseudoRoot(),
                    layer->GetPseudoRoot(),
                    resynched,
                    changedInfo);
                CloseNotice(layer, std::move(changedInfo), std::move(resynched)).Send(m_stage);
                m_transactions.erase(it);
            }
            return true;
        }
        TF_WARN("Tried to close unopened transaction");
        return false;
    }
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdtransaction/AL/usd/transaction/TransactionManager.cpp" line="202">

---

## CloseAll

The `CloseAll` function closes all transactions by clearing the map of Transaction Managers. This function is useful for cleanup purposes.

```c++
//----------------------------------------------------------------------------------------------------------------------
void TransactionManager::CloseAll() { GetManagers().clear(); }
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
