TEMPLATE = subdirs

SUBDIRS += ADLParser

SUBDIRS += PvComponents
PvComponents.depends += ADLParser

SUBDIRS += ADLViewer
ADLViewer.depends += ADLParser PvComponents

