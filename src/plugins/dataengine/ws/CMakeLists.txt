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
        Qt6::Core
)

if (TARGET Qt6::WebSockets)
    target_link_libraries(csdataenginews PRIVATE Qt6::WebSockets)
endif()
