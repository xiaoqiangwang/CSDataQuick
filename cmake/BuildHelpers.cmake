# Main entry functions to create libraries, plugins and executables

set(QT_STATIC $<STREQUAL:$<TARGET_PROPERTY:Qt${QT_VERSION_MAJOR}::Core,TYPE>,STATIC_LIBRARY>)

function(csdq_add_executable target_name)
    cmake_parse_arguments(_arg
    ""
    ""
    "DEPENDS;DEFINES;SOURCES;RESOURCES"
    ${ARGN}
    )

    if (${_arg_UNPARSED_ARGUMENTS})
        message(FATAL_ERROR "Unparsed arguments: " ${_arg_UNPARSED_ARGUMENTS})
    endif()

    if (CMAKE_VERSION VERSION_LESS 3.0)
        qt5_add_resources(resources ${_arg_RESOURCES})
    else()
        set(resources ${_arg_RESOURCES})
    endif()
    add_executable(${target_name} WIN32 MACOSX_BUNDLE ${_arg_SOURCES} ${resources})
    target_link_libraries(${target_name} PRIVATE ${_arg_DEPENDS})

    file(RELATIVE_PATH _relative_libdir "/${CSDQ_INSTALL_BINDIR}" "/${CSDQ_INSTALL_LIBDIR}")
    if(APPLE)
        set_target_properties(${target_name} PROPERTIES
            INSTALL_RPATH "@executable_path/../../../${_relative_libdir}")
    elseif(UNIX)
        SET_TARGET_PROPERTIES(${target_name} PROPERTIES
            INSTALL_RPATH "$ORIGIN/${_relative_libdir}")
    endif()

    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    install(TARGETS ${target_name} DESTINATION ${CSDQ_INSTALL_BINDIR})
endfunction()

function(csdq_add_library target_name)
    cmake_parse_arguments(_arg
    "STATIC"
    ""
    "DEPENDS;PUBLIC_DEPENDS;DEFINES;PUBLIC_DEFINES;SOURCES"
    ${ARGN}
    )

    if (${_arg_UNPARSED_ARGUMENTS})
        message(FATAL_ERROR "Unparsed arguments: " ${_arg_UNPARSED_ARGUMENTS})
    endif()

    set(library_type SHARED)
    if(${QT_STATIC} OR ${_arg_STATIC})
        set(library_type STATIC)
    endif()

    set(depends_scope PRIVATE)
    if(${library_type} STREQUAL "STATIC")
        set(depends_scope PUBLIC)
    endif()

    add_library(${target_name} ${library_type} ${_arg_SOURCES})

    target_include_directories(${target_name} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

    target_compile_definitions(${target_name} PUBLIC ${_arg_PUBLIC_DEFINES})
    target_compile_definitions(${target_name} PRIVATE ${_arg_DEFINES})

    target_link_libraries(${target_name} PUBLIC ${_arg_PUBLIC_DEPENDS})
    target_link_libraries(${target_name} ${depends_scope} ${_arg_DEPENDS})

    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )

    install(TARGETS ${target_name}
        RUNTIME DESTINATION ${CSDQ_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CSDQ_INSTALL_LIBDIR}
    )
endfunction()

function(csdq_add_plugin target_name)
    cmake_parse_arguments(_arg
    "STATIC"
    "TYPE;CLASS_NAME;INTERFACE"
    "DEPENDS;PUBLIC_DEPENDS;DEFINES;PUBLIC_DEFINES;SOURCES"
    ${ARGN}
    )

    if (${_arg_UNPARSED_ARGUMENTS})
        message(FATAL_ERROR "Unparsed arguments: " ${_arg_UNPARSED_ARGUMENTS})
    endif()

    set(library_type MODULE)
    if(${QT_STATIC} OR ${_arg_STATIC})
        set(library_type STATIC)
    endif()

    set(depends_scope PRIVATE)
    if(${library_type} STREQUAL "STATIC")
        set(depends_scope PUBLIC)
        # stub file to initialize static plugin
        set(init_source "${CMAKE_CURRENT_BINARY_DIR}/${target_name}_init.cpp")
        file(GENERATE
            OUTPUT ${init_source}
            CONTENT
            "#include <QtPlugin>\nQ_IMPORT_PLUGIN(${_arg_CLASS_NAME})"
        )
        add_library(${target_name}_init_target OBJECT ${init_source})
        target_link_libraries(${target_name}_init_target PUBLIC Qt${QT_VERSION_MAJOR}::Core)
    endif()

    # plugin target
    add_library(${target_name} ${library_type} ${_arg_SOURCES})

    target_compile_definitions(${target_name} PUBLIC ${_arg_PUBLIC_DEFINES})
    target_compile_definitions(${target_name} PRIVATE ${_arg_DEFINES})
    if(${library_type} STREQUAL "STATIC")
        target_compile_definitions(${target_name} PRIVATE QT_STATICPLUGIN)
    endif()
    target_include_directories(${target_name} PRIVATE $<TARGET_PROPERTY:${_arg_INTERFACE},INCLUDE_DIRECTORIES>)

    target_link_libraries(${target_name} PUBLIC ${_arg_PUBLIC_DEPENDS})
    target_link_libraries(${target_name} ${depends_scope} ${_arg_DEPENDS})
    if(${library_type} STREQUAL "MODULE")
        if(APPLE)
            set_target_properties(${target_name} PROPERTIES SUFFIX ".dylib")
        endif()
        target_link_libraries(${target_name} PRIVATE ${_arg_INTERFACE})
    endif()

    set_target_properties(${target_name} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/${_arg_TYPE})

    install(TARGETS ${target_name}
        LIBRARY DESTINATION ${CSDQ_INSTALL_PLUGINDIR}/${_arg_TYPE})

endfunction()

function(csdq_add_qml_module)
    cmake_parse_arguments(_arg
    ""
    "URI;PLUGIN_TARGET;CLASS_NAME"
    "DEPENDS;DEFINES;SOURCES;RESOURCES;QML_FILES"
    ${ARGN}
    )

    if (${_arg_UNPARSED_ARGUMENTS})
        message(FATAL_ERROR "Unparsed arguments: " ${_arg_UNPARSED_ARGUMENTS})
    endif()


    if (CMAKE_VERSION VERSION_LESS 3.0)
        qt5_add_resources(resources ${_arg_RESOURCES})
    else()
        set(resources ${_arg_RESOURCES})
    endif()

    set(library_type MODULE)
    if(${QT_STATIC})
        set(library_type STATIC)
    endif()

    set(depends_scope PRIVATE)
    if(${library_type} STREQUAL "STATIC")
        set(depends_scope PUBLIC)
    endif()

    string(REPLACE "." "/" qml_path ${_arg_URI})

    add_library(${_arg_PLUGIN_TARGET} ${library_type}
        ${_arg_SOURCES}
        ${_arg_RESOURCES}
        ${_arg_QML_FILES}
    )

    target_compile_definitions(${_arg_PLUGIN_TARGET} PRIVATE ${_arg_DEFINES})

    target_link_libraries(${_arg_PLUGIN_TARGET}
            ${depends_scope}
            ${_arg_DEPENDS}
    )

    set_target_properties(${_arg_PLUGIN_TARGET} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/${qml_path})


    file(RELATIVE_PATH _relative_libdir "/${CSDQ_INSTALL_QMLDIR}/${qml_path}" "/${CSDQ_INSTALL_LIBDIR}")
    if(APPLE)
        set_target_properties(${_arg_PLUGIN_TARGET} PROPERTIES
            INSTALL_RPATH "@loader_path/${_relative_libdir}")
    elseif(UNIX)
        set_target_properties(${_arg_PLUGIN_TARGET} PROPERTIES
            INSTALL_RPATH "$ORIGIN/${_relative_libdir}")
    endif()

    foreach(qml_file ${_arg_QML_FILES})
        list(APPEND DST_QML_FILES ${CMAKE_BINARY_DIR}/qml/${qml_path}/${qml_file})
    endforeach()

    add_custom_target(${_arg_PLUGIN_TARGET}_CopyQMLFiles ALL
        DEPENDS ${DST_QML_FILES})

    foreach(qml_file ${_arg_QML_FILES})
        # make subdirectory if not existing
        get_filename_component(subdir ${qml_file} DIRECTORY)

        if (NOT IS_DIRECTORY ${CMAKE_BINARY_DIR}/qml/${qml_path}/${subdir})
            file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/qml/${qml_path}/${subdir})
        endif()
        # copy qml files prebuild
        add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/qml/${qml_path}/${qml_file}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${qml_file} ${CMAKE_BINARY_DIR}/qml/${qml_path}/${subdir}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endforeach()

    add_dependencies(${_arg_PLUGIN_TARGET} ${_arg_PLUGIN_TARGET}_CopyQMLFiles)

    install(TARGETS ${_arg_PLUGIN_TARGET}  DESTINATION ${CSDQ_INSTALL_QMLDIR}/${qml_path})
    foreach (qml_file ${_arg_QML_FILES})
        get_filename_component(subdir ${qml_file} DIRECTORY)
        install(FILES ${qml_file} DESTINATION ${CSDQ_INSTALL_QMLDIR}/${qml_path}/${subdir})
    endforeach()

endfunction()
