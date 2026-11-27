set(APP csdq_builder)

csdq_add_executable(${APP}
    SOURCES
        main.cpp
    RESOURCES
        main.qrc
    DEPENDS
        Qt6::Gui
        Qt6::Qml
        Qt6::QuickControls2
        csdata
)
