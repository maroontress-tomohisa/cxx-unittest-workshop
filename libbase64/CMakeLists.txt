set(CMAKE_CXX_STANDARD 20)

if(CMAKE_COMPILER_IS_GNUCXX)
    include(CodeCoverage)
endif()

add_library(base64 STATIC
    src/base64/Decoder.cxx
    src/base64/DecodeHelper.cxx)

if(CMAKE_COMPILER_IS_GNUCXX)
    append_coverage_compiler_flags_to_target(base64)
endif()

target_include_directories(base64 PUBLIC include)

if(CMAKE_COMPILER_IS_GNUCXX)
    setup_target_for_coverage_gcovr_html(
        NAME coverage
        EXECUTABLE bash -c "ctest --test-dir base64-test || exit 0")
endif()
