---
title: Getting started
---

This document will cover the process of getting started with the maya-usd repository. We'll cover:

1. The purpose and main functionalities of the repo
2. Important documents to read from the repo
3. How to run and debug the main parts of the repo
4. How to contribute to the repo.

# Purpose and Main Functionalities of the Repo

The 'maya-usd' repository is for creating a Maya plugin and reusable libraries for Pixar's Universal Scene Description (USD). It aims to merge the best features of Pixar's USDMaya and Animal Logic's plugins into one. It provides translation and editing capabilities for Pixar Animation Studios Universal Scene Description (USD).

# Important Documents to Read from the Repo

The repository provides several important documents for understanding and using the repo. These include the [Contributing](doc/CONTRIBUTING.md) guide, the [Building](doc/build.md) guide, the [Coding guidelines](doc/codingGuidelines.md), the [License](doc/LICENSE.md), and the [Plugins documentation](README_DOC.md).

<SwmSnippet path="/build.py" line="141">

---

# Running and Debugging the Main Parts of the Repo

The `Run` function in `build.py` is used to run the specified command in a subprocess. This is used for building and installing the libraries.

```python
def Run(context, cmd):
    """Run the specified command in a subprocess."""
    PrintInfo('Running "{cmd}"'.format(cmd=cmd))

    with codecs.open(context.logFileLocation, "a", "utf-8") as logfile:
        logfile.write("#####################################################################################" + "\n")
        logfile.write("log date: " + datetime.datetime.now().strftime("%Y-%m-%d %H:%M") + "\n")
```

---

</SwmSnippet>

# Contributing to the Repo

Contributions to the repo are welcomed. Detailed guidelines for contributing can be found in the [Contributing](doc/CONTRIBUTING.md) guide. The repo aims to work directly with Pixar and Animal Logic, as well as other key contributors, in order to merge the best of both into a single supported plugin.

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
