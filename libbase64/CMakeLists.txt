set(CMAKE_CXX_STANDARD 20)

add_library(base64 STATIC
    src/base64/Decoder.cxx
    src/base64/DecodeHelper.cxx)

target_include_directories(base64 PUBLIC include)
