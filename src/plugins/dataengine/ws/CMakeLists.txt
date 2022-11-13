csdq_add_plugin(csdataenginews
    TYPE
        csdataengine
    CLASS_NAME
        QCSDataEngineWS
    INTERFACE
        csdata
    SOURCES
        csdataenginews.h
        csdataenginews.cpp
    DEPENDS
        Qt${QT_VERSION_MAJOR}::Core
)

if (TARGET Qt${QT_VERSION_MAJOR}::WebSockets)
    target_link_libraries(csdataenginews PRIVATE Qt${QT_VERSION_MAJOR}::WebSockets)
endif()
