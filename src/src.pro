TEMPLATE = subdirs

SUBDIRS += ADLParser

SUBDIRS += csdatacore

SUBDIRS += csdatacomponents
csdatacomponents.depends += csdatacore ADLParser

SUBDIRS += ADLViewer
ADLViewer.depends += csdatacomponents

SUBDIRS += builder
builder.depends += csdatacomponents
