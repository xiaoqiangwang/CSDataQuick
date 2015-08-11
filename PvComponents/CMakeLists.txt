file (GLOB QML_JS_FILES *.qml *.js qmldir)
macro(COPY_QML_FILES)
    foreach(file IN LISTS ARGN)
        execute_process({CMAKE_COMMAND} -E ${file} ${CMAKE_BINARY_DIR}/PvComponents)
    endforeach()
endmacro()

add_custom_target (PvComponents ALL 
    COMMAND
    SOURCES ${QML_JS_FILES}
)

