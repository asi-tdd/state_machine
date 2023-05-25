cmake_minimum_required(VERSION 3.15)
project(state_machine LANGUAGES CXX)
enable_testing()

include(cmakemodules/gtest_utils.cmake)
build_gtest(deps/googletest dependencies/googletest)

add_subdirectory(state_machine)
