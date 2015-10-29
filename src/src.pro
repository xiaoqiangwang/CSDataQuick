TEMPLATE = subdirs

SUBDIRS += ADLParser

SUBDIRS += csdata

SUBDIRS += csdatacore
csdatacore.depends = csdata

SUBDIRS += csdatacomponents
csdatacomponents.depends += csdatacore ADLParser

SUBDIRS += ADLViewer
ADLViewer.depends += csdatacomponents

SUBDIRS += builder
builder.depends += csdatacomponents
