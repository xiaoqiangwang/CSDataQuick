set(SOURCES
    csdataenginesim.cpp
)

set(HEADERS
    csdataenginesim.h
)

add_library(csdataenginesim STATIC
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(csdataenginesim PRIVATE
    QT_STATICPLUGIN
)

target_link_libraries(csdataenginesim PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)

target_include_directories(csdataenginesim PRIVATE
    $<TARGET_PROPERTY:csdata,INCLUDE_DIRECTORIES>
)
