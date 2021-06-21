set(APP csdq_builder)

csdq_add_executable(${APP}
    SOURCES
        main.cpp
    RESOURCES
        main.qrc
    DEPENDS
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Qml
)
