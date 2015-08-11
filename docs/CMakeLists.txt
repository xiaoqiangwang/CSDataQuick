find_program(QDOC_EXECUTABLE qdoc PATHS ${QT_BINARY_PREFIX})
if(QDOC_EXECUTABLE STREQUAL "QDOC_EXECUTABLE-NOTFOUND")
    message(WARNING "qdoc not found, documentation cannot be built")
else()
    add_custom_target(doc
                      COMMAND ${QDOC_EXECUTABLE}
                      ${CMAKE_CURRENT_SOURCE_DIR}/epicsqml.qdocconf
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                      COMMENT "Generate QDoc..."
                      SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/epicsqml.qdocconf.in)
endif()
