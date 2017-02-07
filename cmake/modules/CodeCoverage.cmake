# Copyright (c) 2012 - 2015, Lars Bilke
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software without
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#
#
# 2012-01-31, Lars Bilke
# - Enable Code Coverage
#
# 2013-09-17, Joakim Söderberg
# - Added support for Clang.
# - Some additional usage instructions.
#
# USAGE:

# 0. (Mac only) If you use Xcode 5.1 make sure to patch geninfo as described here:
#      http://stackoverflow.com/a/22404544/80480
#
# 1. Copy this file into your cmake modules path.
#
# 2. Add the following line to your CMakeLists.txt:
#      INCLUDE(CodeCoverage)
#
# 3. Set compiler flags to turn off optimization and enable coverage:
#    SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
#	 SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
#
# 3. Use the function SETUP_TARGET_FOR_COVERAGE to create a custom make target
#    which runs "make test" and produces a lcov code coverage report:
#    Example:
#	 SETUP_TARGET_FOR_COVERAGE(
#				my_coverage_target  # Name for custom target.
#				coverage            # Name of output directory.
#				)
#
# 4. Build a Debug build:
#	 cmake -DCMAKE_BUILD_TYPE=Debug ..
#	 make
#	 make my_coverage_target
#
#

# Check prereqs
FIND_PROGRAM( GCOV_PATH gcov )
FIND_PROGRAM( LCOV_PATH lcov )
FIND_PROGRAM( GENHTML_PATH genhtml )

IF(NOT GCOV_PATH)
	MESSAGE(FATAL_ERROR "gcov not found! Aborting...")
ENDIF()

IF("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
	IF("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 3)
		MESSAGE(FATAL_ERROR "Clang version must be 3.0.0 or greater! Aborting...")
	ENDIF()
ELSEIF(NOT CMAKE_COMPILER_IS_GNUCXX)
	MESSAGE(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
ENDIF()

IF (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  MESSAGE( WARNING "Code coverage results with an optimized (non-Debug) build may be misleading" )
ENDIF()


# Param _targetname      The name of new the custom make target
# Param _outputname      lcov output is generated as _outputname.info
#                        HTML report is generated in _outputname/index.html
FUNCTION(SETUP_TARGET_FOR_COVERAGE _targetname _outputname)
	IF(NOT LCOV_PATH)
		MESSAGE(FATAL_ERROR "lcov not found! Aborting...")
	ENDIF()

	IF(NOT GENHTML_PATH)
		MESSAGE(FATAL_ERROR "genhtml not found! Aborting...")
	ENDIF()

	SET(initial_info "${CMAKE_BINARY_DIR}/${_outputname}-baseline.info")
	SET(test_info "${CMAKE_BINARY_DIR}/${_outputname}-tests.info")
	SET(all_info "${VARINT_LCOV_OUTPUT_FILE}")
	SET(capture_args --directory . --base-directory '${CMAKE_SOURCE_DIR}' --no-external)

	# Setup target
	ADD_CUSTOM_TARGET(${_targetname}
		COMMAND ${LCOV_PATH} --directory . --zerocounters # Reset lcov counters

		# Capture baseline data. Generates ".gcda" file for uncovered files (too).
		COMMAND ${LCOV_PATH} --capture --initial ${capture_args} -o ${initial_info}

		# Run tests and capture lcov info
		COMMAND make test
		COMMAND ${LCOV_PATH} --capture ${capture_args} -o ${test_info}

		# Merge baseline and test coverage info
		COMMAND ${LCOV_PATH} -a ${initial_info} -a ${test_info} -o ${all_info}

		COMMAND ${LCOV_PATH} --remove ${all_info} "/external/*" -o ${all_info}

		COMMAND ${GENHTML_PATH} -o ${_outputname} ${all_info} # Generate HTML

		# Cleanup intermediate files
		COMMAND ${CMAKE_COMMAND} -E remove ${initial_info} ${test_info}

		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
	)

	# Show info where to find the report
	ADD_CUSTOM_COMMAND(TARGET ${_targetname} POST_BUILD
		COMMAND ;
		COMMENT "Open ./${_outputname}/index.html in your browser to view the coverage report."
	)
ENDFUNCTION()
