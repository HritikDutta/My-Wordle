@echo off

set includes= /I src /I dependencies\glad\include /I dependencies\wglext\include /I dependencies\stb\include

set libs= Shell32.lib                     ^
          User32.lib                      ^
          Gdi32.lib                       ^
          OpenGL32.lib                    ^
          msvcrt.lib                      ^
          Comdlg32.lib                    ^
          dependencies\glad\lib\glad.lib  ^
          dependencies\stb\lib\stb.lib

if "%1"=="release" (
    set defines= /DGN_USE_OPENGL /DGN_PLATFORM_WINDOWS /DGN_RELEASE /DNDEBUG
    set compile_flags= /O2 /EHsc /std:c++17 /cgthreads8 /MP7 /GL
    set link_flags= /NODEFAULTLIB:LIBCMT /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup /LTCG
) else (
    set defines= /DGN_USE_OPENGL /DGN_PLATFORM_WINDOWS /DGN_DEBUG
    set compile_flags= /Zi /EHsc /std:c++17 /cgthreads8 /MP7 /GL
    set link_flags= /DEBUG /NODEFAULTLIB:LIBCMT /LTCG
)

rem Remove existing files
del *.exe *.pdb

rem Source
cl /c %compile_flags% src/containers/*.cpp %defines% %includes% & ^
cl /c %compile_flags% src/fileio/*.cpp %defines% %includes% & ^
cl /c %compile_flags% src/serialization/json/*.cpp %defines% %includes% & ^
cl /c %compile_flags% src/math/constants.cpp %defines% %includes% & ^
cl /c %compile_flags% src/core/*.cpp %defines% %includes% & ^
cl /c %compile_flags% src/platform/*.cpp %defines% %includes% & ^
cl /c %compile_flags% src/physics/*.cpp %defines% %includes% & ^
cl /c %compile_flags% src/engine/*.cpp %defines% %includes% & ^
cl /c %compile_flags% src/graphics/*.cpp %defines% %includes% & ^
cl /c %compile_flags% src/main.cpp %defines% %includes%

rem Resources
rc resources.rc

rem Link and Make Executable
link *.obj %objs% %libs% *.res /OUT:wordle.exe %link_flags%

rem Delete Intermediate Files
del *.obj
del *.res