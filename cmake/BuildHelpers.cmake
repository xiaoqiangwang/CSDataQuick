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

    if(APPLE)
        set_target_properties(${target_name} PROPERTIES
            INSTALL_RPATH "@executable_path/../../../../lib")
    elseif(UNIX)
        SET_TARGET_PROPERTIES(${target_name} PROPERTIES
            INSTALL_RPATH "$ORIGIN/../lib")
    endif()

    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    install(TARGETS ${target_name} DESTINATION ${KDE_INSTALL_BINDIR})
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
        RUNTIME DESTINATION ${KDE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${KDE_INSTALL_LIBDIR}
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
        LIBRARY DESTINATION ${KDE_INSTALL_PLUGINDIR}/plugins/${_arg_TYPE})

endfunction()
