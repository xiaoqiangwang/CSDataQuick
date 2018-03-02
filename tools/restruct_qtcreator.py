#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
This script re-constucts the Qt Creator package as distributed by Qt installer on Windows and macOS.
On Linux it expects the Qt Creator is a local installation.
"""
import os
import platform
import re
import sys
import shutil
import subprocess

if sys.hexversion < 0x03000000:
    subprocess.getoutput = lambda cmd: subprocess.check_output(cmd, shell=True)

app_bundle = sys.argv[1]
target_path = sys.argv[2]

bin_dir = os.path.join(target_path, 'bin')
lib_dir = os.path.join(target_path, 'lib')
libexec_dir = os.path.join(target_path, 'libexec')
plugins_dir = os.path.join(target_path, 'plugins')
qml_dir = os.path.join(target_path, 'qml')


def restruct_app_bundle():
    bundle_name = os.path.basename(app_bundle)
    if not bundle_name.endswith('.app'):
        print('Not a valid app bundle')
        return

    # Copy the app bundle to bin
    os.makedirs(bin_dir, exist_ok=True)
    shutil.copytree(app_bundle, os.path.join(bin_dir, bundle_name), symlinks=True)

    # Move out Qt Frameworks
    os.makedirs(lib_dir, exist_ok=True)
    src_frameworks_dir = os.path.join(bin_dir, bundle_name, 'Contents', 'Frameworks')
    for d in os.listdir(src_frameworks_dir):
        if re.match(b'Qt.*\.framework', d):
            shutil.move(os.path.join(src_frameworks_dir, d), lib_dir)

    # Move out Qt plugins
    os.makedirs(plugins_dir, exist_ok=True)
    src_plugins_dir = os.path.join(bin_dir, bundle_name, 'Contents', 'PlugIns')
    for plugin in ['bearer', 'designer', 'iconengines', 'imageformats',
                   'platforms', 'printsupport', 'sqldrivers']:
        shutil.move(os.path.join(src_plugins_dir, plugin), plugins_dir)

    # Move out QtQuick modules
    os.makedirs(qml_dir, exist_ok=True)
    src_qml_dir = os.path.join(bin_dir, bundle_name, 'Contents', 'Imports', 'qtquick2')
    for m in os.listdir(src_qml_dir):
        shutil.move(os.path.join(src_qml_dir, m), qml_dir)

    # Fix rpath and qt.conf
    for root, dirs, files in os.walk(os.path.join(bin_dir, bundle_name)):
        for file in files:
            fname = os.path.join(root, file)
            if os.path.islink(fname):
                continue
            if  file == 'qml2puppet' or os.path.basename(root) == 'MacOS' or os.path.splitext(file)[1] == '.dylib':
                cmd = 'install_name_tool -add_rpath "@loader_path/%s" "%s"' % (os.path.relpath(lib_dir, root), fname)
                os.system(cmd)
            elif file == 'qt.conf':
                if os.path.basename(root) == 'Resources':
                    open(fname, 'w').write('[Paths]\nPrefix = %s\n' % os.path.relpath(target_path, os.path.dirname(root)))
                else:
                    open(fname, 'w').write('[Paths]\nPrefix = %s\n' % os.path.relpath(target_path, root))


def restruct_windows():
    # Copy the entire directory
    for d in ['bin', 'lib', 'share']:
        shutil.copytree(os.path.join(app_bundle, d), os.path.join(target_path, d))

    # Move out Qt plugins
    shutil.move(os.path.join(bin_dir, 'plugins'), target_path)

    # Move out QtQuick modules
    shutil.move(os.path.join(bin_dir, 'qml'), target_path)

    # Fix qt.conf
    open(os.path.join(bin_dir, 'qt.conf'), 'w').write('[Paths]\nPrefix = ..\n')


def restruct_linux():
    qmake = sys.argv[3]
    # Copy the entire directory
    for d in ['bin', 'lib', 'libexec', 'share']:
        shutil.copytree(os.path.join(app_bundle, d), os.path.join(target_path, d))

    # Copy Qt libs
    qtlibs_dir = subprocess.getoutput('%s -query QT_INSTALL_LIBS' % qmake).strip()
    for lib in ['Core', 'Gui', 'Widgets', 'Concurrent', 'Network', 'PrintSupport',
                'Qml', 'Quick', 'QuickWidgets', 'Xml', 'Svg', 'XcbQpa', 'Sql']:
        shutil.copy(os.path.join(qtlibs_dir, 'libQt5%s.so.5'%lib), lib_dir)

    # Copy Qt plugins
    qtplugins_dir = subprocess.getoutput('%s -query QT_INSTALL_PLUGINS' % qmake).strip()
    os.makedirs(plugins_dir)
    for plugin in ['bearer', 'designer', 'iconengines', 'imageformats',
                   'platforms', 'sqldrivers', 'xcbglintegrations']:
        shutil.copytree(os.path.join(qtplugins_dir, plugin), os.path.join(plugins_dir, plugin))
    # Copy QtQuick modules
    qtqml_dir = subprocess.getoutput('%s -query QT_INSTALL_QML' % qmake).strip()
    shutil.copytree(qtqml_dir, qml_dir)
    # Fix qt.conf
    open(os.path.join(bin_dir, 'qt.conf'), 'w').write('[Paths]\nPrefix = ..\n')
    open(os.path.join(libexec_dir, 'qtcreator', 'qt.conf'), 'w').write('[Paths]\nPrefix = ../..\n')
    # Fix rpath of qtcreator and executibles under libexec
    cmd = "chrpath -r '$ORIGIN/../lib/qtcreator:$ORIGIN/../lib:' " + os.path.join(bin_dir, 'qtcreator')
    os.system(cmd)
    for f in os.listdir(os.path.join(libexec_dir, 'qtcreator')):
        cmd = "chrpath -r '$ORIGIN/../../lib/qtcreator:$ORIGIN/../../lib:' " + os.path.join(libexec_dir, 'qtcreator', f)
        os.system(cmd)


if platform.system() == 'Darwin':
    restruct_app_bundle()
elif platform.system() == 'Windows':
    restruct_windows()
elif platform.system() == 'Linux':
    restruct_linux()
