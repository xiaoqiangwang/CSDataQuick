csdq_add_plugin(csdataengineca csdata
    TYPE
        csdataengine
    CLASS_NAME
        QCSDataEngineCA
    INTERFACE
        csdata
    SOURCES
        csdataengineca.h
        csdataengineca.cpp
    DEFINES
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    DEPENDS
        Qt${QT_VERSION_MAJOR}::Core
        EPICS::ca
)
