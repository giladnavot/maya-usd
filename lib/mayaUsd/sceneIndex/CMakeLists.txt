# -----------------------------------------------------------------------------
# sources
# -----------------------------------------------------------------------------
target_sources(${PROJECT_NAME} 
    PRIVATE
        proxyShapeSceneIndexPlugin.cpp
)

set(HEADERS
    proxyShapeSceneIndexPlugin.h
)

# -----------------------------------------------------------------------------
# promote headers
# -----------------------------------------------------------------------------
mayaUsd_promoteHeaderList(HEADERS ${HEADERS} SUBDIR sceneIndex)

# -----------------------------------------------------------------------------
# install
# -----------------------------------------------------------------------------

install(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}/sceneIndex
)

# -----------------------------------------------------------------------------
# plugInfo
# -----------------------------------------------------------------------------

set(PLUG_INFO_LIBRARY_PATH ${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
set(PLUG_INFO_DIR ${CMAKE_INSTALL_PREFIX}/lib/usd/sceneIndex/resources)

configure_file(
    plugInfo.json
    ${CMAKE_CURRENT_BINARY_DIR}/plugInfo.json
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plugInfo.json
    DESTINATION ${PLUG_INFO_DIR}
)

