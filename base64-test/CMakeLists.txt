set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_executable(base64-test
    DecoderTest.cxx
    DecodeHelperTest.cxx)

target_include_directories(base64-test PRIVATE base64)

target_link_libraries(base64-test base64 GTest::gtest_main)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(base64-test gcov)
endif()

include(GoogleTest)

gtest_discover_tests(base64-test)
