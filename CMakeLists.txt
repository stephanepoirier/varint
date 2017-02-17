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

set(VARINT_TESTS_OPTION_MSG "Build tests.")
option(VARINT_TESTS "${VARINT_TESTS_OPTION_MSG}" ON)
option(VARINT_COVERAGE "Perform code coverage analysis." OFF)

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(EXTERNAL_DIR "${PROJECT_SOURCE_DIR}/external")
set(VARINT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(INSTALL_INCLUDE_DIR "include")
set(LIB_NAME ${PROJECT_NAME})

################################################################################
#                        General compilation flags                             #
################################################################################

set(CMAKE_C_COMPILE_OBJECT "${CMAKE_CXX_COMPILE_OBJECT}") #Compile C as CXX
set(CMAKE_CXX_STANDARD 11) # C++11 required
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Disable compiler-specific extensions

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
	"${EXTERNAL_DIR}/bitpacking/simdintegratedbitpacking.c"
	"${EXTERNAL_DIR}/bitpacking/util.cpp"
	"${SOURCE_DIR}/BasicSet.cpp"
	"${SOURCE_DIR}/CompressedDeltaChunk.cpp"
	"${SOURCE_DIR}/CompressedSet.cpp"
	"${SOURCE_DIR}/LazyAndSet.cpp"
	"${SOURCE_DIR}/LazyOrSet.cpp"
)

add_library(${LIB_NAME} "${SOURCE_FILES}")

# Sets C++ standard >= C++11 to current project and those that import it
target_compile_features(${LIB_NAME} PUBLIC cxx_nonstatic_member_init)

list(APPEND INCLUDE_DIRS ${VARINT_INCLUDE_DIR} ${EXTERNAL_DIR})
foreach(INCLUDE_DIR ${INCLUDE_DIRS})
	target_include_directories(${LIB_NAME}
		PUBLIC
	    $<BUILD_INTERFACE:${INCLUDE_DIR}>
	    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
	)
	install(
    DIRECTORY "${INCLUDE_DIR}/" DESTINATION ${INSTALL_INCLUDE_DIR}
    FILES_MATCHING
      PATTERN "*.h*"
      PATTERN ".hpp"
  )
endforeach()

set(CONFIG_NAME "${LIB_NAME}Config")
install(
	TARGETS ${LIB_NAME}
	EXPORT ${CONFIG_NAME}
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib/static
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION ${INSTALL_INCLUDE_DIR}
)
install(EXPORT ${CONFIG_NAME} DESTINATION "share/${LIB_NAME}/cmake")
export(TARGETS ${LIB_NAME} FILE "${CONFIG_NAME}.cmake")
export(PACKAGE ${PROJECT_NAME})

################################################################################
#                           Coverage analysis setup                            #
################################################################################

if(VARINT_COVERAGE)
	status("Coverage analysis is enabled. Configuring coverage.")
	if (NOT VARINT_TESTS)
		message(WARNING "Coverage depends on tests. Enabling tests.")
		set(VARINT_TESTS ON CACHE BOOL "${VARINT_TESTS_OPTION_MSG}" FORCE) # Tests are required for coverage
	endif()

  include(CodeCoverage)
	target_compile_options(${LIB_NAME} PUBLIC --coverage)
	target_link_libraries(${LIB_NAME} PUBLIC gcov)
	setup_target_for_coverage("coverage")
endif()

################################################################################
#                                Tests setup                                   #
################################################################################

if (VARINT_TESTS)
	status("Tests are enabled. Adding tests subdirectory.")
	enable_testing()
	add_subdirectory(test)
endif()
