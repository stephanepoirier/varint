################################################################################
#
# CMake file for varint library tests.
#
# Author: Stephane Poirier (stephane.poirier01@gmail.com)
#
################################################################################

project(varint-tests VERSION 0.1)

status("Configuring test dependencies")
add_subdirectory("external") # Sets "gtest_SOURCE_DIR", "gtest" and "gtest_main"

list(APPEND TEST_FILENAMES
	"CompressedSetTests.cpp"
	"sanity_check.cpp"
)
foreach(TEST_FILENAME ${TEST_FILENAMES})
	get_filename_component(TEST_NAME ${TEST_FILENAME} NAME_WE)
	add_executable("${TEST_NAME}" "${TEST_FILENAME}")
	add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")
	target_link_libraries("${TEST_NAME}" gtest gtest_main ${LIB_NAME})
endforeach()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
