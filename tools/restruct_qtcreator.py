import logging
import os
import re
import sys
import shutil

app_bundle = sys.argv[1]
target_path = sys.argv[2]
bundle_name = os.path.basename(app_bundle)

if not bundle_name.endswith('.app'):
    print('Not a valid app bundle')

bin_dir = os.path.join(target_path, 'bin')
os.makedirs(bin_dir, exist_ok=True)

lib_dir = os.path.join(target_path, 'lib')
os.makedirs(lib_dir, exist_ok=True)

plugins_dir = os.path.join(target_path, 'plugins')
os.makedirs(plugins_dir, exist_ok=True)

qml_dir = os.path.join(target_path, 'qml')
os.makedirs(qml_dir, exist_ok=True)

# Copy the rest of the app bundle
shutil.copytree(app_bundle, os.path.join(bin_dir, bundle_name), symlinks=True)

# Move out Qt Frameworks
src_frameworks_dir = os.path.join(bin_dir, bundle_name, 'Contents', 'Frameworks')
for d in os.listdir(src_frameworks_dir):
    if re.match('Qt.*\.framework', d):
        shutil.move(os.path.join(src_frameworks_dir, d), lib_dir)

# Move out Qt plugins
src_plugins_dir = os.path.join(bin_dir, bundle_name, 'Contents', 'PlugIns')
for plugin in ['bearer', 'designer', 'iconengines', 'imageformats', 
        'platforms', 'printsupport', 'sqldrivers']:
    shutil.move(os.path.join(src_plugins_dir, plugin), plugins_dir)

# Move out QtQuick modules
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

