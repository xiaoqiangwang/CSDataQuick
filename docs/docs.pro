TEMPLATE = aux
QT += help

QMAKE_DOCS = $$PWD/epicsqml.qdocconf

OTHER_FILES = \
    $$PWD/epicsqml.qdocconf \
    $$PWD/epicsqml.qdoc

qtPrepareTool(QDOC, qdoc)

QDOC_INDEXES += -indexdir $$[QT_INSTALL_DOCS/get]

html.commands = $$QDOC $$QDOC_INDEXES $$QMAKE_DOCS

QMAKE_EXTRA_TARGETS += html
