set(SOURCES
    QMLParser.cpp
)

set(HEADERS
    QMLParser.h
)

add_library(QMLParser STATIC
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(QMLParser PRIVATE
    QT_STATICPLUGIN
)

target_include_directories(QMLParser PRIVATE
    $<TARGET_PROPERTY:csparser,INCLUDE_DIRECTORIES>
)

target_link_libraries(QMLParser PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)
