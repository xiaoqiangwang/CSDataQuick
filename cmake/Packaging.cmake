set (CPACK_PACKAGE_VENDOR "Paul Scherrer Institut")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Control System Data in QtQuick")
set (CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/README.rst)
seT (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set (CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.rst)
set (CPACK_PACKAGE_VERSION_MAJOR ${CSDATAQUICK_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${CSDATAQUICK_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${CSDATAQUICK_VERSION_RELEASE})
set (CPACK_PACKAGE_EXECUTABLES Viewer "Quick Viewer" builder "Quick Builder" QtCreator "Qt Quick Designer")
set (CPACK_MONOLITHIC_INSTALL TRUE)

if (APPLE)
    set (CPACK_GENERATOR "DragNDrop")
    set (CPACK_PACKAGING_INSTALL_PREFIX "/CSDataQuick")
elseif (UNIX)
    set (CPACK_GENERATOR "TGZ")
endif()

# Include Qt Creator
if (QTC_PACKAGE)
    set (CPACK_INSTALLED_DIRECTORIES ${QTC_PACKAGE} ".")
    if (APPLE)
        set (QTC_RESOURCE "bin/Qt Creator.app/Contents/Resources")
    elseif (WIN32)
        set (QTC_RESOURCE "share/qtcreator")
    else ()
        set (QTC_RESOURCE "share/qtcreator")
    endif()
    # Copy templates
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/share/qtcreator/templates/wizards/files/csdataquick
        DESTINATION "${QTC_RESOURCE}/templates/wizards/files")
    install(FILES ${CMAKE_SOURCE_DIR}/share/qtcreator/externaltools/viewer.xml
        DESTINATION "${QTC_RESOURCE}/externaltools")
endif()

set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(CPACK_SOURCE_IGNORE_FILES
        \\\\.git/
        /build-.*/
        ".*~$"
)

include(CPack)
