TEMPLATE = subdirs

SUBDIRS += ADLParser

SUBDIRS += csdatacore

SUBDIRS += csdatacomponents
csdatacomponents.depends += ADLParser

SUBDIRS += ADLViewer
ADLViewer.depends += ADLParser csdatacomponents

