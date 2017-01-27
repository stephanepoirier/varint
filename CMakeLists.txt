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

set(SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(INSTALL_INCLUDE_DIR "include")
set(LIB_NAME ${PROJECT_NAME})
set(CONFIG_NAME "${LIB_NAME}Config")

################################################################################
#                            Compilation flags                                 #
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
#                             Subprojects setup                                #
################################################################################

option(WITH_TESTS "Build tests." ON)
if (WITH_TESTS)
	status("Tests are enabled. Adding tests subdirectory.")
	enable_testing()
	add_subdirectory(test)
endif()
