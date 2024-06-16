---
title: Registering a Translator Process
---

This document will cover the process of registering a translator in the Maya-USD plugin, which includes:

1. Registering the translator
2. Generating the script
3. Initializing the parser
4. Inserting the node into the map
5. Ending the process.

```mermaid
graph TD;
subgraph plugin/al/mayautils/AL/maya/utils
  registerTranslator:::mainFlowStyle --> generateScript
end
subgraph plugin/al/mayautils/AL/maya/utils
  generateScript:::mainFlowStyle --> initParser
end
subgraph plugin/al/mayautils/AL/maya/utils
  initParser:::mainFlowStyle --> insert
end
subgraph plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h
  insert:::mainFlowStyle --> end
end
  end:::mainFlowStyle --> ...

 classDef mainFlowStyle color:#000000,fill:#7CB9F4
  classDef rootsStyle color:#000000,fill:#00FFF4
```

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/FileTranslatorBase.h" line="1">

---

# Registering the translator

The process begins with the `registerTranslator` function. This function is responsible for registering a new translator in the Maya-USD plugin.

```c
//
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/FileTranslatorOptions.cpp" line="521">

---

# Generating the script

The `generateScript` function is called next. This function generates a script for each exporter/importer option. It creates GUI controls for the options and adds entries into the optionParser.

```c++
//----------------------------------------------------------------------------------------------------------------------
MStatus
FileTranslatorOptions::generateScript(OptionsParser& optionParser, MString& defaultOptionString)
{
    initParser(optionParser);

    // first generate a collection of methods to create, edit, and query each separate option. For
    // each exporter/importer option, we will generate three methods:
    //
    //   proc create_myOptionName();              ///< creates the GUI control for the option
    //   proc post_myOptionName(string $value);   ///< set the value in the control from the parsed
    //   option string proc string build_myOptionName();        ///< get the value from the control,
    //   and return it as a text string "myOptionName=<value>"
    //
    // We will also add in some entries into the optionParser, which will be used later on when
    // using the exporter. This option parser will know about the option names (both the 'nice'
    // names and the actual option name), as well as the associated data type. This will be able to
    // split apart the option string of the form "option1=10;option2=hello;option3=true"
    //
    auto itf = m_frames.begin();
    auto endf = m_frames.end();
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/FileTranslatorOptions.cpp" line="443">

---

# Initializing the parser

The `initParser` function is then called. This function initializes the parser with the options generated in the previous step.

```c++
//----------------------------------------------------------------------------------------------------------------------
MStatus FileTranslatorOptions::initParser(OptionsParser& optionParser)
{
    // first generate a collection of methods to create, edit, and query each separate option. For
    // each exporter/importer option, we will generate three methods:
    //
    //   proc create_myOptionName();              ///< creates the GUI control for the option
    //   proc post_myOptionName(string $value);   ///< set the value in the control from the parsed
    //   option string proc string build_myOptionName();        ///< get the value from the control,
    //   and return it as a text string "myOptionName=<value>"
    //
    // We will also add in some entries into the optionParser, which will be used later on when
    // using the exporter. This option parser will know about the option names (both the 'nice'
    // names and the actual option name), as well as the associated data type. This will be able to
    // split apart the option string of the form "option1=10;option2=hello;option3=true"
    //
    auto itf = m_frames.begin();
    auto endf = m_frames.end();
    for (; itf != endf; ++itf) {
        auto ito = itf->m_options.begin();
        auto endo = itf->m_options.end();
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/mayautils/AL/maya/utils/MObjectMap.h" line="38">

---

# Inserting the node into the map

The `insert` function is called to insert a node into the map. This function checks if the node already exists in the map, and if not, it adds the node to the map.

```c
    /// \brief  insert a node into the map.
    /// \param  fn the function set attached to the node to insert
    /// \return true if the node had already been added, false if the node was added.
    inline bool insert(const MFnDependencyNode& fn)
    {
#if AL_UTILS_ENABLE_SIMD
        union
        {
            __m128i sse;
            guid    uuid;
        };
        fn.uuid().get(uuid.uuid);
        bool contains = m_nodeMap.find(sse) != m_nodeMap.end();
        if (!contains)
            m_nodeMap.insert(std::make_pair(sse, fn.object()));
#else
        guid uuid;
        fn.uuid().get(uuid.uuid);
        bool contains = m_nodeMap.find(uuid) != m_nodeMap.end();
        if (!contains)
            m_nodeMap.insert(std::make_pair(uuid, fn.object()));
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/al/usdmayautils/AL/usdmaya/utils/AnimationTranslator.h" line="322">

---

# Ending the process

The process ends with the `end` function. This function returns an iterator to the end of the commonTransformAttributes array.

```c
    inline std::array<MObject, transformAttributesCount>::const_iterator end() const
    {
        return m_commonTransformAttributes.cend();
    }
```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
