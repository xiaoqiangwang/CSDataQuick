TEMPLATE = aux
QT += help

QMAKE_DOCS = $$PWD/epicsqml.qdocconf

OTHER_FILES = \
    $$PWD/epicsqml.qdocconf \
    $$PWD/epicsqml.qdoc

html.commands = qdoc $$QMAKE_DOCS

QMAKE_EXTRA_TARGETS += html
