GET_TARGET_PROPERTY(QMAKE_EXECUTABLE ${Qt5Core_QMAKE_EXECUTABLE} LOCATION)
EXEC_PROGRAM( ${QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_DOCS" OUTPUT_VARIABLE QT_DOC_DIR )
EXEC_PROGRAM( ${QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_BINS" OUTPUT_VARIABLE QT_BIN_DIR )

find_program(QDOC_EXECUTABLE qdoc HINTS ${QT_BIN_DIR})
if(QDOC_EXECUTABLE STREQUAL "QDOC_EXECUTABLE-NOTFOUND")
    message(WARNING "qdoc not found, documentation cannot be built")
else()
    file (GLOB QDOC_FILES *.qdocconf *.qdoc style/*)
    add_custom_target(html-offline
        COMMAND ${QDOC_EXECUTABLE} --indexdir ${QT_DOC_DIR} --outputdir ${CMAKE_CURRENT_BINARY_DIR}/html-offline ${CMAKE_CURRENT_SOURCE_DIR}/csdataquick-offline.qdocconf
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                      COMMENT "Generate offline documents..."
                      SOURCES ${QDOC_FILES})
    add_custom_target(html-online
        COMMAND ${QDOC_EXECUTABLE} --indexdir ${QT_DOC_DIR} --outputdir ${CMAKE_CURRENT_BINARY_DIR}/html-online ${CMAKE_CURRENT_SOURCE_DIR}/csdataquick-online.qdocconf
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                      COMMENT "Generate online documents..."
                      SOURCES ${QDOC_FILES})
endif()
