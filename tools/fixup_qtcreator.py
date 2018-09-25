#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
This script re-constucts the Qt Creator installation to include Qt libraries, plugins, QtQuick.

Windows:
    |-- bin
    |   |-- qt.conf -> Prefix=..
    |   |-- qt dlls
    |   |-- csdataquick executibles
    |   |-- csdataquick dlls
    |   |-- qt-creator executibles
    |   |-- qt-creator dlls
    |
    |-- lib
    |   |-- qtcreator
    |       |-- plugins
    |
    |-- plugins
    |   |-- qt plugins
    |   |-- csdaquick plguins
    |
    |-- qml
        |-- CSDataQuick
        |-- QtQuick

Linux:
    |-- bin
    |   |-- qt.conf -> Prefix=..
    |   |-- csdataquick executibles
    |   |-- qtcreator
    |
    |-- lib
    |   |-- qt shared libraries
    |   |-- csdataquick shared libraries
    |   |-- qtcreator
    |       |-- qtcreator shared libraries
    |       |-- plugins
    |
    |-- libexec
    |   |-- qtcreator
    |       |-- qt.conf -> Prefix=../..
    |       |-- qml2puppet
    |
    |-- plugins
    |   |-- csdaquick plguins
    |   |-- qt plugins
    |
    |-- qml
        |-- CSDataQuick
        |-- QtQuick


macOS:
    |-- bin
    |   |-- csdataquick app bundles
    |   |   |-- Contents
    |   |       |-- Resources
    |   |           |-- qt.conf -> Prefix=../../../..
    |   |-- Qt Creator.app
    |       |-- Contents
    |           |-- Resources
    |               |-- qt.conf -> Prefix=../../../..
    |               |-- qmldesigner
    |                   |-- qt.conf  -> Prefix=../../../../..
    |
    |-- lib
    |   |-- qt frameworks
    |   |-- csdataquick shared libraries
    |
    |-- plugins
    |   |-- qt plugins
    |   |-- csdataquick plugins
    |
    |-- qml
        |-- CSDataQuick
        |-- QtQuick
"""
import argparse
import glob
import os
import platform
import re
import sys
import shutil
import subprocess

if sys.hexversion < 0x03000000:
    if sys.hexversion < 0x02070000:
        subprocess.getoutput = lambda cmd: subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True).communicate()[0]
    else:
        subprocess.getoutput = lambda cmd: subprocess.check_output(cmd, shell=True)

parser = argparse.ArgumentParser(description='Fixup Qt and Qt Creator for packaging')
parser.add_argument('--target', required=True, help='target path')
parser.add_argument('--qtcreator', help='qt creator path')
parser.add_argument('--qmake', required=True, help='qmake file path')
args = parser.parse_args(sys.argv[1:])

qtcreator_path = args.qtcreator
target_path = args.target
qmake = args.qmake

bin_dir = os.path.join(target_path, 'bin')
lib_dir = os.path.join(target_path, 'lib')
libexec_dir = os.path.join(target_path, 'libexec')
plugins_dir = os.path.join(target_path, 'plugins')
qml_dir = os.path.join(target_path, 'qml')


def smartCopy(src, dst, follow_symlinks=True, ignore=None):
    """
    same as shell cp command. If *src* is a file, it is copied into *dst* if dst is an existing directory
    or as file *dst*. If *src* is a directory, it is copied recursively into *dst* if dst is an existing
    directory or as as directory *dst*.
    """
    if os.path.isdir(dst):
        dst = os.path.join(dst, os.path.basename(src))

    if not follow_symlinks and os.path.islink(src):
        os.symlink(os.readlink(src), dst)
    else:
        if os.path.isdir(src):
            shutil.copytree(src, dst, symlinks=not follow_symlinks, ignore=ignore)
        else:
            shutil.copyfile(src, dst)
            shutil.copystat(src, dst)
    return dst


def deployQtLibraries():
    libs = ['Core', 'Gui', 'Widgets', 'Concurrent', 'Network', 'PrintSupport', 'Script',
            'Qml', 'Quick', 'QuickWidgets', 'QuickControls2', 'QuickTemplates2', 'QuickParticles',
            'Xml', 'Svg', 'Sql', 'Help']
    qtlibs_dir = subprocess.getoutput('%s -query QT_INSTALL_LIBS' % qmake).strip()
    dst_dir = lib_dir
    lib_pattern = 'libQt5%s.so*'
    ignore_pattern = None

    if platform.system() == 'Darwin':
        lib_pattern = 'Qt%s.framework'
        ignore_pattern = shutil.ignore_patterns('Headers', '*_debug', '*.prl')
    elif platform.system() == 'Windows':
        qtlibs_dir = subprocess.getoutput('%s -query QT_INSTALL_BINS' % qmake).strip()
        dst_dir = bin_dir
        lib_pattern = 'Qt5%s.dll'
    elif platform.system() == 'Linux':
        libs += ['XcbQpa']

    if not os.path.exists(dst_dir):
        os.makedirs(dst_dir)

    for lib in libs:
        for file in glob.glob(os.path.join(qtlibs_dir, lib_pattern%lib)):
            smartCopy(file, dst_dir, follow_symlinks=False, ignore=ignore_pattern)

    if platform.system() == 'Windows':
        for lib in ['libEGL.dll', 'libGLESv2.dll']:
            smartCopy(os.path.join(qtlibs_dir, lib), dst_dir)


def deployQtPlugins():
    plugins = ['bearer', 'designer', 'iconengines', 'imageformats',
               'platforms', 'sqldrivers']
    qtplugins_dir = subprocess.getoutput('%s -query QT_INSTALL_PLUGINS' % qmake).strip()
    if not os.path.exists(plugins_dir):
        os.makedirs(plugins_dir)

    if platform.system() == 'Linux':
        plugins += ['xcbglintegrations']

    for plugin in plugins:
        shutil.copytree(os.path.join(qtplugins_dir, plugin),
                        os.path.join(plugins_dir, plugin),
                        symlinks=True,
                        ignore=shutil.ignore_patterns('*_debug.dylib', '*.dylib.dSYM', '*.pdb'))
        # remove debug version on windows
        if platform.system() == 'Windows':
            # After sorting the debug version "<pluginname>d.dll" will be 
            # immedietly after the release version "<pluginname>.dll".
            # It is then quick to remove every 2nd file from this list.
            dlls = sorted(os.listdir(os.path.join(plugins_dir, plugin)))[1::2]
            for dll in dlls:
                os.remove(os.path.join(plugins_dir, plugin, dll))


def deployQtQuick():
    qtqml_dir = subprocess.getoutput('%s -query QT_INSTALL_QML' % qmake).strip()
    if not os.path.exists(qml_dir):
        os.makedirs(qml_dir)

    for qml in ['Qt', 'QtQml', 'QtGraphicalEffects', 'QtQuick', 'QtQuick.2']:
        shutil.copytree(os.path.join(qtqml_dir, qml),
                        os.path.join(qml_dir, qml),
                        symlinks=True,
                        ignore=shutil.ignore_patterns('*_debug.dylib', '*.dylib.dSYM', '*plugind.dll','*.pdb'))


def deployQt():
    # Copy Qt libraries
    deployQtLibraries()

    # Copy Qt plugins
    deployQtPlugins()

    # Copy QtQuick modules
    deployQtQuick()


def restruct_macos():
    bundle_name = os.path.basename(qtcreator_path)
    if not bundle_name.endswith('.app'):
        print('Not a valid app bundle')
        return

    # Copy the app bundle to bin
    if not os.path.exists(bin_dir):
        os.makedirs(bin_dir)
    shutil.copytree(qtcreator_path, os.path.join(bin_dir, bundle_name), symlinks=True)

    # Fix rpath
    for root, dirs, files in os.walk(os.path.join(bin_dir, bundle_name)):
        for file in files:
            fname = os.path.join(root, file)
            if os.path.islink(fname):
                continue
            if  file == 'qml2puppet' or os.path.basename(root) == 'MacOS' or os.path.splitext(file)[1] == '.dylib':
                cmd = 'install_name_tool -add_rpath "@loader_path/%s" "%s"' % (os.path.relpath(lib_dir, root), fname)
                subprocess.call(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)

    # Fix qt.conf
    open(os.path.join(bin_dir, bundle_name, 'Contents', 'Resources', 'qt.conf'), 'w').write('[Paths]\nPrefix = ../../..\n')
    open(os.path.join(bin_dir, bundle_name, 'Contents', 'Resources', 'qmldesigner', 'qt.conf'), 'w').write('[Paths]\nPrefix = ../../../../..\n')

def restruct_windows():
    # Copy the entire directory
    for d in ['bin', 'lib', 'share']:
        shutil.copytree(os.path.join(qtcreator_path, d), os.path.join(target_path, d))

    # Fix qt.conf
    open(os.path.join(bin_dir, 'qt.conf'), 'w').write('[Paths]\nPrefix = ..\n')


def restruct_linux():
    # Copy the entire directory
    for d in ['bin', 'lib', 'libexec', 'share']:
        shutil.copytree(os.path.join(qtcreator_path, d), os.path.join(target_path, d), symlinks=True)

    # Fix rpath of qtcreator and executibles under libexec
    cmd = "chrpath -r '$ORIGIN/../lib/qtcreator:$ORIGIN/../lib:' " + os.path.join(bin_dir, 'qtcreator')
    subprocess.call(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
    for f in os.listdir(os.path.join(libexec_dir, 'qtcreator')):
        cmd = "chrpath -r '$ORIGIN/../../lib/qtcreator:$ORIGIN/../../lib:' " + os.path.join(libexec_dir, 'qtcreator', f)
        subprocess.call(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)

    # Fix qt.conf
    open(os.path.join(bin_dir, 'qt.conf'), 'w').write('[Paths]\nPrefix = ..\n')
    open(os.path.join(libexec_dir, 'qtcreator', 'qt.conf'), 'w').write('[Paths]\nPrefix = ../..\n')


if qtcreator_path and os.path.exists(qtcreator_path):
    if platform.system() == 'Darwin':
        restruct_macos()
    elif platform.system() == 'Windows':
        restruct_windows()
    elif platform.system() == 'Linux':
        restruct_linux()
# Fixup Qt
deployQt()

