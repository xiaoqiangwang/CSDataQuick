set (EXAMPLES
    widgets.qml
    LED.gif
    gallery/demo_adimage.qml
    gallery/demo_arc.qml
    gallery/demo_bar.qml
    gallery/demo_byte.qml
    gallery/demo_choicebutton.qml
    gallery/demo_image.qml
    gallery/demo_indicator.qml
    gallery/demo_menu.qml
    gallery/demo_messagebutton.qml
    gallery/demo_meter.qml
    gallery/demo_oval.qml
    gallery/demo_polygon.qml
    gallery/demo_polyline.qml
    gallery/demo_rect.qml
    gallery/demo_slider.qml
    gallery/demo_text.qml
    gallery/demo_textentry.qml
    gallery/demo_textupdate.qml
    gallery/ColorModeGroup.qml
    gallery/ColorPalette.qml
    gallery/DirectionGroup.qml
    gallery/EdgeStyleGroup.qml
    gallery/FillModeGroup.qml
    gallery/FillStyleGroup.qml
    gallery/LabelStyleGroup.qml
    gallery/OrientationGroup.qml
    gallery/StackingGroup.qml
    gallery/TextAlignmentGroup.qml
    gallery/TextFormatGroup.qml
    gallery/main.qml
)

add_custom_target (Examples ALL 
    SOURCES ${EXAMPLES})

