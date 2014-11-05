set (EXAMPLES
    widgets.qml
    image.qml
    LED.gif
    example.db    
    GraphicsDemo.qml
)

add_custom_target (Examples ALL 
    SOURCES ${EXAMPLES})

