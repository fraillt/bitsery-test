cmake_minimum_required(VERSION 3.16)


project(bitsery-test VERSION 0.0.0 LANGUAGES CXX C)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(test_sdfu
    #${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/test_sdfu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sdfu_codec.cpp
)

target_include_directories(test_sdfu PRIVATE ./)

target_compile_features(test_sdfu PRIVATE cxx_variadic_templates)


set_target_properties(test_sdfu PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS ON)

#target_include_directories(test_sdfu PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}/include
#    ${CMAKE_CURRENT_SOURCE_DIR}/include/implementation
#)

