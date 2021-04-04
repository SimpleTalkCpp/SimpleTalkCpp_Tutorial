mkdir _prebuild\x64\protobuf
cd    _prebuild\x64\protobuf

cmake -G "Visual Studio 15 2017 Win64" -Dprotobuf_MSVC_STATIC_RUNTIME=OFF ../../../protobuf-master/cmake

@pause