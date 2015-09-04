set (EXAMPLES
    widgets.qml
    LED.gif
    example.db    
)

add_custom_target (Examples ALL 
    SOURCES ${EXAMPLES})

