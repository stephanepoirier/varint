################################################################################
#
# Main CMake file for the varint project.
#
# Author: Stephane Poirier (stephane.poirier01@gmail.com)
#
################################################################################

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(varint VERSION 0.1 LANGUAGES CXX)

include("${PROJECT_SOURCE_DIR}/cmake/utils.cmake")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

option(VARINT_TESTS "Build tests." ON)
option(VARINT_COVERAGE "Perform code coverage analysis." ON)
set(VARINT_LCOV_OUTPUT_FILE "${CMAKE_BINARY_DIR}/coverage-all.info" CACHE STRING "lcov coverage output file.")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS None Debug Release RelWithDebInfo MinSizeRel)

set(SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(INSTALL_INCLUDE_DIR "include")
set(LIB_NAME ${PROJECT_NAME})
set(CONFIG_NAME "${LIB_NAME}Config")


################################################################################
#                        General compilation flags                             #
################################################################################

set(CMAKE_C_COMPILE_OBJECT "${CMAKE_CXX_COMPILE_OBJECT}") #Compile C as CXX
set(CMAKE_CXX_STANDARD 11) #C++11 required

if(CMAKE_COMPILER_IS_GNUCXX OR CYGWIN OR APPLE OR MINGW)
	add_definitions(-Wall -pedantic) #Warning level
	add_definitions(-msse2) #SSE2
elseif(MSVC)
	add_definitions(/W4) #Warning level
	add_definitions(/arch:SSE2) #SSE2
else()
	message(WARNING "Unknown build tool. Compilation flags are not configured.")
endif()


################################################################################
#                           Library configuration                              #
################################################################################
status("Configuring library '${LIB_NAME}'")

list(APPEND	SOURCE_FILES
	"${SOURCE_DIR}/CompressedSet.cpp"
	"${SOURCE_DIR}/CompressedDeltaChunk.cpp"
	"${SOURCE_DIR}/bitpacking/simdintegratedbitpacking.c"
	"${SOURCE_DIR}/bitpacking/util.cpp"
	"${SOURCE_DIR}/LazyAndSet.cpp"
	"${SOURCE_DIR}/LazyOrSet.cpp"
	"${SOURCE_DIR}/BasicSet.cpp"
)

add_library(${LIB_NAME} "${SOURCE_FILES}")
target_include_directories(${LIB_NAME}
	PUBLIC
    $<BUILD_INTERFACE:${SOURCE_DIR}>
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

status("Configuring installation instructions")

install(
	TARGETS ${LIB_NAME}
	EXPORT ${CONFIG_NAME}
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib/static
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION ${INSTALL_INCLUDE_DIR}
)
# TODO: Rework headers to support private ones and #include prefix
install(DIRECTORY "${SOURCE_DIR}/" DESTINATION ${INSTALL_INCLUDE_DIR} FILES_MATCHING PATTERN "*.h")
install(EXPORT ${CONFIG_NAME} DESTINATION "share/${LIB_NAME}/cmake")

export(TARGETS ${LIB_NAME} FILE "${CONFIG_NAME}.cmake")


################################################################################
#                           Coverage analysis setup                            #
################################################################################

if(VARINT_COVERAGE)
	status("Coverage analysis is enabled. Configuring coverage.")
  include(CodeCoverage)
	set_target_properties(${LIB_NAME} PROPERTIES
		COMPILE_FLAGS "${CMAKE_CXX_FLAGS} --coverage"
		LINK_FLAGS "--coverage"
	)
	target_link_libraries(${LIB_NAME} gcov)
	setup_target_for_coverage(coverage coverage)
endif()


################################################################################
#                                Tests setup                                   #
################################################################################

if (VARINT_TESTS)
	status("Tests are enabled. Adding tests subdirectory.")
	enable_testing()
	add_subdirectory(test)
endif()
