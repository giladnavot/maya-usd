---
title: Understanding pxrUsdMayaGL
---

pxrUsdMayaGL is a part of the Maya USD plugin that handles the drawing of USD data in Maya's viewport. It provides the functionality to render USD data using Maya's native drawing capabilities. This includes handling different types of USD data such as meshes, instancers, and assemblies. The test files in the pxrUsdMayaGL directory are used to verify the correct rendering of USD data in Maya's viewport.

<SwmSnippet path="/plugin/pxr/maya/lib/pxrUsdMayaGL/testenv/testPxrUsdMayaGLInstancerDraw.py" line="28">

---

# Test Setup

This section of the code sets up the test environment. It loads the necessary plugins, sets up the workspace, and ensures that the test USD data is authored correctly.

```python
class testPxrUsdMayaGLInstancerDraw(imageUtils.ImageDiffingTestCase):

    @classmethod
    def setUpClass(cls):
        # The test USD data is authored Z-up, so make sure Maya is configured
        # that way too.
        cmds.upAxis(axis='z')

        cls._testName = 'InstancerDrawTest'
        inputPath = fixturesUtils.setUpClass(
            __file__, initializeStandalone=False, loadPlugin=False)

        cmds.loadPlugin('pxrUsd')

        cls._testDir = os.path.abspath('.')
        cls._inputDir = os.path.join(inputPath, cls._testName)

        # To control where the rendered images are written, we force Maya to
        # use the test directory as the workspace.
        cmds.workspace(cls._testDir, o=True)

```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/pxrUsdMayaGL/testenv/testPxrUsdMayaGLInstancerDraw.py" line="53">

---

# Rendering Test

This section of the code performs the actual rendering test. It sets up the hardware renderer, specifies the output image format and render mode, and then performs the render. It also handles the output of the rendered image.

```python
    def _WriteViewportImage(self, outputImageName, suffix, baselineSuffix = None):
        if baselineSuffix is None:
            baselineSuffix = suffix

        # Make sure the hardware renderer is available
        MAYA_RENDERER_NAME = 'mayaHardware2'
        mayaRenderers = cmds.renderer(query=True,
            namesOfAvailableRenderers=True)
        self.assertIn(MAYA_RENDERER_NAME, mayaRenderers)

        # Make it the current renderer.
        cmds.setAttr('defaultRenderGlobals.currentRenderer', MAYA_RENDERER_NAME,
            type='string')
        # Set the image format to PNG.
        cmds.setAttr('defaultRenderGlobals.imageFormat', 32)
        # Set the render mode to shaded and textured.
        cmds.setAttr('hardwareRenderingGlobals.renderMode', 4)
        # Specify the output image prefix. The path to it is built from the
        # workspace directory.
        cmds.setAttr('defaultRenderGlobals.imageFilePrefix',
            '%s_%s' % (outputImageName, suffix),
```

---

</SwmSnippet>

<SwmSnippet path="/plugin/pxr/maya/lib/pxrUsdMayaGL/testenv/testPxrUsdMayaGLInstancerDraw.py" line="120">

---

# Test Scenarios

This section of the code sets up specific test scenarios. It opens a Maya file, sets up the viewport, and generates images for comparison. It also handles changes to the scene, such as deleting instances and reloading the scene.

```python
    def testGenerateImages(self):
        cmds.file(os.path.abspath('InstancerDrawTest.ma'),
                open=True, force=True)

        # The cards rendering colors in older versions of Maya is lighter,
        suffix = ''
        if mayaUtils.mayaMajorVersion() <= 2024:
            suffix = '_v1'

        # Draw in VP2 at current frame.
        self._SetModelPanelsToViewport2()
        self._WriteViewportImage("InstancerTest", "initial")

        # Load assembly in "Cards" to use cards drawmode.
        cmds.assembly("CubeModel", edit=True, active="Cards")
        self._WriteViewportImage("InstancerTest", "cards" + suffix)

        # Change the time; this will change the instancer positions.
        cmds.currentTime(50, edit=True)
        self._WriteViewportImage("InstancerTest", "frame50" + suffix)

```

---

</SwmSnippet>

&nbsp;

_This is an auto-generated document by Swimm AI 🌊 and has not yet been verified by a human_

<SwmMeta version="3.0.0" repo-id="Z2l0aHViJTNBJTNBbWF5YS11c2QlM0ElM0FnaWxhZG5hdm90" repo-name="maya-usd"><sup>Powered by [Swimm](/)</sup></SwmMeta>
