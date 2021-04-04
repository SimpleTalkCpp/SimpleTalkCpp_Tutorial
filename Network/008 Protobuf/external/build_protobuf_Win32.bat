mkdir _prebuild\Win32\protobuf
cd    _prebuild\Win32\protobuf

cmake -G "Visual Studio 15 2017" -Dprotobuf_MSVC_STATIC_RUNTIME=OFF ../../../protobuf-master/cmake

@pause