set EPICS_BASE=%PREFIX%\epics
if %ARCH%==32 (
   set EPICS_HOST_ARCH=win32-x86-static
) else if %ARCH%==64 (
    set EPICS_HOST_ARCH=windows-x64-static
)

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" ..
nmake
xcopy /E /I bin %LIBRARY_PREFIX%\bin
xcopy /E /I plugins %LIBRARY_PREFIX%\plugins
xcopy /E /I qml %LIBRARY_PREFIX%\qml
