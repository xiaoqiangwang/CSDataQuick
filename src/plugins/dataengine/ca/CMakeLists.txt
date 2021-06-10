set(SOURCES
    csdataengineca.cpp
)

set(HEADERS
    csdataengineca.h
)

add_library(csdataengineca STATIC
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(csdataengineca PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    QT_STATICPLUGIN
)

target_include_directories(csdataengineca PRIVATE
    $<TARGET_PROPERTY:csdata,INCLUDE_DIRECTORIES>
)

target_link_libraries(csdataengineca PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    EPICS::ca
)
