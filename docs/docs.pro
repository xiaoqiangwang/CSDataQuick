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

html.commands = $$QDOC $$QDOC_INDEXES $$QMAKE_DOCS

QMAKE_EXTRA_TARGETS += html
