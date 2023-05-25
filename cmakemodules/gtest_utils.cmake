# This can be used to add and build the googletest (and googlemock)
# project to your CMakeLists.txt file.
# SOURCE_DIR should be the directory that googletest was cloned to.
# BUILD_DIR should the the directory where cmake build cache is. You should use the same one
# for all calls to this function, to avoid needlessly rebuilding googletest.
# After this executes, the calling cmake project should be able to use the
#    gtest, gtest_main, and gmock targets.
function(build_gtest SOURCE_DIR BUILD_DIR)
    if (GHSMULTI)
        add_definitions(--exceptions --c++14)
    endif()
    option(INSTALL_GTEST "" OFF)
    if (MSVC)
        # Prevent GoogleTest from overriding our compiler/linker options
        # when building with Visual Studio
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
    add_subdirectory(${SOURCE_DIR} ${BUILD_DIR} EXCLUDE_FROM_ALL)
endfunction(build_gtest)

# This will add a gtest test executable in a common way so that it can be run by ctest and emit results
# TEST_NAME should be the desired name of the test executable.
# SOURCES should be list of source files to compile with the executable
# LIBRARIES should be list of libraries to link (gtest, gmock, and gtest_main will be automatically linked)
# OUT_DIR is directory where build and test output will be placed. Each xunit file will be named TEST_NAME.xunit.xml
function(add_gtest TEST_NAME SOURCES INCLUDES LIBRARIES OUT_DIR)
    add_executable(${TEST_NAME} ${SOURCES})
    get_filename_component(DATA_PATH ${SOURCES} DIRECTORY )
    target_compile_definitions(${TEST_NAME} PUBLIC UNIT_TEST_DATA="${DATA_PATH}/Data/" __ignorecplusplus)
    target_include_directories(${TEST_NAME} PUBLIC ${INCLUDES})
    target_link_libraries(${TEST_NAME}
            gtest
            gmock
            gtest_main
            ${LIBRARIES}
            )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME}
            --gtest_shuffle
            "--gtest_output=xml:${OUT_DIR}-$<CONFIG>/${TEST_NAME}.xunit.xml"
            )
endfunction(add_gtest)