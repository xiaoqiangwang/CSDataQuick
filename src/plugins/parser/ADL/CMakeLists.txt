set(SOURCES
    element.cpp
    ADLParser.cpp
)

set(HEADERS
    common.h
    element.h
    ADLParser.h
)

add_library(ADLParser STATIC
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(ADLParser PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    QT_STATICPLUGIN
)

target_include_directories(ADLParser PRIVATE
    $<TARGET_PROPERTY:csparser,INCLUDE_DIRECTORIES>
)

target_link_libraries(ADLParser PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)
