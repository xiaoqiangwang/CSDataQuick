TEMPLATE = aux
QT += help

QMAKE_DOCS = $$PWD/csdataquick.qdocconf

OTHER_FILES = \
    $$PWD/csdataquick.qdocconf \
    $$PWD/csdataquick.qdoc \
    $$PWD/csdata.qml\
    $$PWD/cscomponents.qml

qtPrepareTool(QDOC, qdoc)

QDOC_INDEXES += -indexdir $$[QT_INSTALL_DOCS/get]

QDOC_OUTPUTS += -outputdir $$OUT_PWD/html

html.commands = $$QDOC $$QDOC_INDEXES $$QDOC_OUTPUTS $$QMAKE_DOCS

QMAKE_EXTRA_TARGETS += html
