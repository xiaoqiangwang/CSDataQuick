set(SOURCES
    csdataenginelocal.cpp
)

set(HEADERS
    csdataenginelocal.h
)

add_library(csdataenginelocal STATIC
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(csdataenginelocal PRIVATE
    QT_STATICPLUGIN
)

target_link_libraries(csdataenginelocal PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(csdataenginelocal PRIVATE
    $<TARGET_PROPERTY:csdata,INCLUDE_DIRECTORIES>
)
