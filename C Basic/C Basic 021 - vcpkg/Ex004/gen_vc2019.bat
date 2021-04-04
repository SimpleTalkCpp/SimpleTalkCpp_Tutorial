@REM #change current directory to this file

@rem change driver letter
@%~d0

@rem change current directory
@cd %~dp0

call .\vcpkg\bootstrap-vcpkg.bat

vcpkg\vcpkg install fmt:x64-windows
vcpkg\vcpkg install imgui[sdl2-binding]:x64-windows --recurse
vcpkg\vcpkg install imgui[opengl3-glew-binding]:x64-windows

mkdir _build
cmake -B _build .

@pause