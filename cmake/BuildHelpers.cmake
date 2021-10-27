# Main entry functions to create libraries, plugins and executables.
# They take care of output path, installation and linking different
# parts in case of using static Qt.

# Check whether using statically built Qt
get_target_property(_qt_lib_type Qt${QT_VERSION_MAJOR}::Core TYPE)
if (_qt_lib_type STREQUAL "STATIC_LIBRARY")
    set(QT_STATIC TRUE)
endif()

# Function to create executable targets.
function(csdq_add_executable target_name)
    cmake_parse_arguments(_arg
    ""
    ""
    "DEPENDS;DEFINES;SOURCES;RESOURCES"
    ${ARGN}
    )

    if (_arg_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unparsed arguments: " ${_arg_UNPARSED_ARGUMENTS})
    endif()

    if (CMAKE_VERSION VERSION_LESS 3.0)
        qt5_add_resources(resources ${_arg_RESOURCES})
    else()
        set(resources ${_arg_RESOURCES})
    endif()
    add_executable(${target_name} WIN32 MACOSX_BUNDLE ${_arg_SOURCES} ${resources})
    target_link_libraries(${target_name} PRIVATE ${_arg_DEPENDS})

    if(QT_STATIC)
        list(REMOVE_DUPLICATES _arg_DEPENDS)
        foreach(_depend IN LISTS _arg_DEPENDS)
            # Initialize plugins that are built by the depending library.
            set(class_names_regular
                "$<GENEX_EVAL:$<TARGET_PROPERTY:${_depend},PLUGINS>>")

            set(out_file_path "${CMAKE_CURRENT_BINARY_DIR}/${target_name}_plugin_imports_${_depend}.cpp")

            file(GENERATE OUTPUT "${out_file_path}" CONTENT
    "// This file is auto-generated. Do not edit.
#include <QtPlugin>
Q_IMPORT_PLUGIN($<JOIN:${class_names_regular},)\nQ_IMPORT_PLUGIN(>)"
                CONDITION "$<NOT:$<STREQUAL:${class_names_regular},>>"
            )
            # CMake versions earlier than 3.18.0 can't find the generated file for some reason,
            # failing at generation phase.
            # Explicitly marking the file as GENERATED fixes the issue.
            set_source_files_properties("${out_file_path}" PROPERTIES GENERATED TRUE)
            target_sources(${target_name} PRIVATE
                "$<$<NOT:$<STREQUAL:${class_names_regular},>>:${out_file_path}>"
            )
        endforeach()

        csdq_import_qml_plugins(${target_name})
    endif()

    if (EMSCRIPTEN)
        # Configure WebAssembly build settings
        # See mkspecs/wasm-emscripten/qmake.conf qtbase/cmake/QtWasmHelpers.cmake
        target_link_libraries(${target_name} PUBLIC
            "-s FULL_ES2=1"
            "-s FULL_ES3=1"
            "-s USE_WEBGL2=1"
            "-s EXIT_RUNTIME=1"
            "-s EXPORTED_RUNTIME_METHODS=[UTF16ToString,stringToUTF16]"
            "--bind"
            "-s FETCH=1"
            "-s ALLOW_MEMORY_GROWTH=1"
            "-s DISABLE_EXCEPTION_CATCHING=0"
        )

        # Copy Qt HTML/JS launch files
        get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake LOCATION)
        execute_process(
            COMMAND ${_qmake_executable} -query QT_INSTALL_PLUGINS
            OUTPUT_VARIABLE _qt_plugins_dir
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        set(APPNAME ${target_name}) # Substitutes in wasm_shell.html
        configure_file("${_qt_plugins_dir}/platforms/wasm_shell.html" ${CMAKE_BINARY_DIR}/bin/${target_name}.html)
        configure_file("${_qt_plugins_dir}/platforms/qtloader.js" ${CMAKE_BINARY_DIR}/bin/qtloader.js COPYONLY)
        configure_file("${_qt_plugins_dir}/platforms/qtlogo.svg" ${CMAKE_BINARY_DIR}/bin/qtlogo.svg COPYONLY)
    endif()

    # RPATH to locate libraries at runtime
    file(RELATIVE_PATH _relative_libdir "/${CSDQ_INSTALL_BINDIR}" "/${CSDQ_INSTALL_LIBDIR}")
    if(APPLE)
        set_target_properties(${target_name} PROPERTIES
            INSTALL_RPATH "@executable_path/../../../${_relative_libdir}")
    elseif(UNIX)
        set_target_properties(${target_name} PROPERTIES
            INSTALL_RPATH "$ORIGIN/${_relative_libdir}")
    endif()

    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    install(TARGETS ${target_name} DESTINATION ${CSDQ_INSTALL_BINDIR})
endfunction()

# Function to create library targets.
function(csdq_add_library target_name)
    cmake_parse_arguments(_arg
    ""
    ""
    "DEPENDS;PUBLIC_DEPENDS;DEFINES;PUBLIC_DEFINES;SOURCES"
    ${ARGN}
    )

    if (_arg_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unparsed arguments: " ${_arg_UNPARSED_ARGUMENTS})
    endif()

    set(library_type SHARED)
    if(QT_STATIC)
        set(library_type STATIC)
    endif()

    set(depends_scope PRIVATE)
    if(library_type STREQUAL "STATIC")
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

# Function to create plugin targets.
function(csdq_add_plugin target_name)
    cmake_parse_arguments(_arg
    "STATIC"
    "TYPE;CLASS_NAME;INTERFACE"
    "DEPENDS;PUBLIC_DEPENDS;DEFINES;PUBLIC_DEFINES;SOURCES"
    ${ARGN}
    )

    if (_arg_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unparsed arguments: " ${_arg_UNPARSED_ARGUMENTS})
    endif()

    set(library_type MODULE)
    if(QT_STATIC OR _arg_STATIC)
        set(library_type STATIC)
    endif()

    set(depends_scope PRIVATE)
    if(library_type STREQUAL "STATIC")
        set(depends_scope PUBLIC)
    endif()

    add_library(${target_name} ${library_type} ${_arg_SOURCES})

    target_compile_definitions(${target_name} PUBLIC ${_arg_PUBLIC_DEFINES})
    target_compile_definitions(${target_name} PRIVATE ${_arg_DEFINES})

    # In static build, append this plugin to the corresponding library's PLUGINS property.
    # The executables, which link to the library, will need to reference these plugins
    # with "Q_IMPORT_PLUGIN" macros.
    if(library_type STREQUAL "STATIC")
        target_compile_definitions(${target_name} PRIVATE QT_STATICPLUGIN)
        target_link_libraries(${_arg_INTERFACE} ${depends_scope} ${target_name})
        set_property(TARGET ${_arg_INTERFACE} APPEND PROPERTY
            PLUGINS ${_arg_CLASS_NAME})
    endif()

    # Include library's interface header files.
    target_include_directories(${target_name} PRIVATE $<TARGET_PROPERTY:${_arg_INTERFACE},INCLUDE_DIRECTORIES>)

    target_link_libraries(${target_name} PUBLIC ${_arg_PUBLIC_DEPENDS})
    target_link_libraries(${target_name} ${depends_scope} ${_arg_DEPENDS})

    if(library_type STREQUAL "MODULE")
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

# Function to create qml modules with plugins.
function(csdq_add_qml_module)
    cmake_parse_arguments(_arg
    ""
    "URI;PLUGIN_TARGET;CLASS_NAME"
    "DEPENDS;DEFINES;SOURCES;RESOURCES;QML_FILES"
    ${ARGN}
    )

    if (_arg_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unparsed arguments: " ${_arg_UNPARSED_ARGUMENTS})
    endif()

    if (CMAKE_VERSION VERSION_LESS 3.0)
        qt5_add_resources(resources ${_arg_RESOURCES})
    else()
        set(resources ${_arg_RESOURCES})
    endif()

    set(library_type MODULE)
    if(QT_STATIC)
        set(library_type STATIC)
    endif()

    set(depends_scope PRIVATE)
    if(library_type STREQUAL "STATIC")
        set(depends_scope PUBLIC)
    endif()

    # QML module path derives from URI, e.g. CSDataQuick.Data results CSDataQuick/Data
    string(REPLACE "." "/" qml_path ${_arg_URI})

    add_library(${_arg_PLUGIN_TARGET} ${library_type}
        ${_arg_SOURCES}
        ${_arg_RESOURCES}
    )

    target_compile_definitions(${_arg_PLUGIN_TARGET} PRIVATE ${_arg_DEFINES})

    target_link_libraries(${_arg_PLUGIN_TARGET} ${depends_scope} ${_arg_DEPENDS})

    if(library_type STREQUAL "STATIC")
        # Qt specific for static qml plugins
        target_compile_definitions(${_arg_PLUGIN_TARGET} PRIVATE QT_STATICPLUGIN)
        set_target_properties(${_arg_PLUGIN_TARGET} PROPERTIES
            AUTOMOC_MOC_OPTIONS "-Muri=${_arg_URI}"
        )

        # Create one resource file from QML files and set to the plugin's RESOURCES property.
        # This resource file be included by dependent targets calling function csdq_import_qml_plugins.
        set(_auto_qml_qrc ${CMAKE_CURRENT_BINARY_DIR}/${_arg_PLUGIN_TARGET}_auto_qml.qrc)

        # Use WRITE instead of GENERATE so that to create this file at configure phase.
        # Otherwise probably due to missing dependency specification, this file is not yet created
        # when generating dependent targets.
        file(WRITE ${_auto_qml_qrc}
"<!-- This file is auto-generated. Do not edit.-->
<RCC>
  <qresource prefix=\"/qt-project.org/imports/${qml_path}\">
")
        foreach(qml_file ${_arg_QML_FILES})
            file(APPEND ${_auto_qml_qrc}
"    <file alias=\"${qml_file}\">${CMAKE_CURRENT_SOURCE_DIR}/${qml_file}</file>
")
        endforeach()

        file(APPEND ${_auto_qml_qrc}
"  </qresource>
</RCC>
")
        set_target_properties(${_arg_PLUGIN_TARGET} PROPERTIES RESOURCES ${_auto_qml_qrc})
    else()
        set_target_properties(${_arg_PLUGIN_TARGET} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml/${qml_path})

        # RPATH to locate libraries at runtime
        file(RELATIVE_PATH _relative_libdir "/${CSDQ_INSTALL_QMLDIR}/${qml_path}" "/${CSDQ_INSTALL_LIBDIR}")
        if(APPLE)
            set_target_properties(${_arg_PLUGIN_TARGET} PROPERTIES
                INSTALL_RPATH "@loader_path/${_relative_libdir}")
        elseif(UNIX)
            set_target_properties(${_arg_PLUGIN_TARGET} PROPERTIES
                INSTALL_RPATH "$ORIGIN/${_relative_libdir}")
        endif()

        foreach (qml_file ${_arg_QML_FILES})
            get_filename_component(subdir ${qml_file} DIRECTORY)
            install(FILES ${qml_file} DESTINATION ${CSDQ_INSTALL_QMLDIR}/${qml_path}/${subdir})
        endforeach()
    endif()

    foreach(qml_file ${_arg_QML_FILES})
        get_filename_component(qml_subdir ${qml_file} DIRECTORY)
        set(qml_dst_path ${CMAKE_BINARY_DIR}/qml/${qml_path})

        # copy qml files so qmlimportscannner can find at generate stage
        file(MAKE_DIRECTORY ${qml_dst_path}/${qml_subdir})
        #file(COPY ${qml_file} DESTINATION ${qml_dst_path}/${qml_subdir})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy ${qml_file} ${qml_dst_path}/${qml_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        # update qml files at build stage
        add_custom_command(
            OUTPUT ${qml_dst_path}/${qml_file}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${qml_file} ${qml_dst_path}/${qml_file}
            DEPENDS ${qml_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        list(APPEND DST_QML_FILES ${qml_dst_path}/${qml_file})
    endforeach()

    # Make target depend on qml files so that they get copied when modified.
    # This proves convenient during development.
    add_custom_target(${_arg_PLUGIN_TARGET}_CopyQMLFiles ALL DEPENDS ${DST_QML_FILES})
    add_dependencies(${_arg_PLUGIN_TARGET} ${_arg_PLUGIN_TARGET}_CopyQMLFiles)

    install(TARGETS ${_arg_PLUGIN_TARGET}  DESTINATION ${CSDQ_INSTALL_QMLDIR}/${qml_path})
endfunction()

# Function for target to search and link depending qml plugins.
function(csdq_import_qml_plugins target)
    # Find location of qmlimportscanner.
    get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake LOCATION)
    execute_process(
        COMMAND ${_qmake_executable} -query QT_INSTALL_BINS
        OUTPUT_VARIABLE _qt_bin_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(
        COMMAND ${_qmake_executable} -query QT_INSTALL_QML
        OUTPUT_VARIABLE _qt_qml_dir
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    find_program(tool_path qmlimportscanner HINTS ${_qt_bin_dir})

    if(NOT tool_path)
        message(WARNING "qmlimportscanner not found, not able to link qml plugins")
        return()
    endif()

    # Dump qml imports to variable
    execute_process(
        COMMAND ${tool_path}
            -rootPath ${CMAKE_CURRENT_SOURCE_DIR}
            -importPath ${_qt_qml_dir}
            -importPath ${CMAKE_BINARY_DIR}/qml
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE qml_dependencies_json
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT qml_dependencies_json)
        return()
    endif()

    # Original source
    #   OlivierLDff/QtStaticCMake/QtStaticCMake.cmake
    #   qtbase/cmake/QtExecutableHelpers.cmake
    string(REGEX MATCHALL "\\{([^\\}]*)\\}" qml_import_json_obj ${qml_dependencies_json})

    # Loop over json objects and extract the following for each qml module
    #   "classname" for Q_IMPORT_PLUGIN macro
    #   "plugin" for find_library
    #   "path" as hint if available
    foreach(json_object ${qml_import_json_obj})
        string(REGEX MATCH "\"classname\"\\: \"([a-zA-Z0-9]*)\"" temp ${json_object})
        if(NOT CMAKE_MATCH_1 OR CMAKE_MATCH_1 IN_LIST qml_dependencies_plugins)
            continue()
        endif()

        set(class_name ${CMAKE_MATCH_1})
        list(APPEND qml_dependencies_plugins ${class_name})

        string(REGEX MATCH "\"plugin\"\\: \"([a-zA-Z0-9_]*)\"" temp ${json_object})
        set(${class_name}_plugin ${CMAKE_MATCH_1})

        string(REGEX MATCH "\"path\"\\: \"([^\"]*)\"" temp ${json_object})
        set(${class_name}_path ${CMAKE_MATCH_1})
    endforeach()

    # Create source file to import qml plugins.
    set(out_file_path "${CMAKE_CURRENT_BINARY_DIR}/${target}_qml_plugin_imports.cpp")
    file(GENERATE
        OUTPUT "${out_file_path}"
        CONTENT "// This file is auto-generated. Do not edit.
#include <QtPlugin>
Q_IMPORT_PLUGIN($<JOIN:${qml_dependencies_plugins},)\nQ_IMPORT_PLUGIN(>)"
        CONDITION "$<NOT:$<STREQUAL:${qml_dependencies_plugins},>>"
    )
    # CMake versions earlier than 3.18.0 can't find the generated file for some reason,
    # failing at generation phase.
    # Explicitly marking the file as GENERATED fixes the issue.
    set_source_files_properties("${out_file_path}" PROPERTIES GENERATED TRUE)
    target_sources(${target} PRIVATE
        "$<$<NOT:$<STREQUAL:${qml_dependencies_plugins},>>:${out_file_path}>"
    )

    # Link to qml plugins
    foreach(plugin ${qml_dependencies_plugins})
        find_library(lib_${plugin}_plugin
            NAMES ${${plugin}_plugin} lib${${plugin}_plugin}
            PATHS ${${plugin}_path})
        if (lib_${plugin}_plugin)
            message(${lib_${plugin}_plugin})
            target_link_libraries(${target} PRIVATE ${lib_${plugin}_plugin})
        elseif(TARGET ${${plugin}_plugin})
            target_sources(${target} PRIVATE $<TARGET_PROPERTY:${${plugin}_plugin},RESOURCES>)
            target_link_libraries(${target} PRIVATE ${${plugin}_plugin})
        endif()
    endforeach()

    # Extra libraries for QtQml.WorkerScript on Qt 5.15+
    if(TARGET Qt${QT_VERSION_MAJOR}::QmlWorkerScript)
        target_link_libraries(${target} PRIVATE Qt${QT_VERSION_MAJOR}::QmlWorkerScript)
    endif()

endfunction()
