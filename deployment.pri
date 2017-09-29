equals(TEMPLATE, app) {
    target.path = $${PREFIX}/bin
    DESTDIR = $$top_builddir/bin
}
equals(TEMPLATE, lib) {
    CONFIG(plugin) {
        target.path = $${PREFIX}/plugins/csdataengine
        DESTDIR = $$top_builddir/plugins/csdataengine
    } else:win32 {
        target.path = $${PREFIX}/bin
        DLLDESTDIR = $$top_builddir/bin
    } else {
        target.path = $${PREFIX}/lib
        DESTDIR = $$top_builddir/lib
    }
}
export(target.path)

INSTALLS += target
export(INSTALLS)

