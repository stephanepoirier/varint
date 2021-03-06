################################################################################
#
# varint tests external dependencies
#
# Author: Stephane Poirier (stephane.poirier01@gmail.com)
#
################################################################################

project(varint-tests-external VERSION 0.1)

status("Importing Google Test")

# Due to GoogleTest issue #606 (https://github.com/google/googletest/issues/606),
# we have to use a recent non-release snapshot (51143d5b). Test with 1.9.0 when it goes online.
set(GTEST_VERSION "51143d5b62521f71020ada4ba1b6b44f3a6749bb")
set(GTEST_ARCHIVE_URL "https://github.com/google/googletest/archive/${GTEST_VERSION}.zip")
set(GTEST_ARCHIVE_SHA1 "db8325d5308e0b29ff4eb104f386ada134fc4e4d")

set(GTEST_PROJECT_NAME "gtest-tmp")
set(GTEST_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GTEST_PROJECT_NAME}")
set(GTEST_TMP_DIR  "${GTEST_ROOT_DIR}/tmp")
set(GTEST_BIN_DIR  "${GTEST_ROOT_DIR}/bin")
set(GTEST_ARCHIVE_DEST_PATH "${GTEST_TMP_DIR}/${GTEST_VERSION}.zip")

status("Looking for a cached archive at '${GTEST_ARCHIVE_DEST_PATH}'.")

if (NOT EXISTS ${GTEST_ARCHIVE_DEST_PATH})
	status("Archive not found. Downloading from '${GTEST_ARCHIVE_URL}'.")

	file(
		DOWNLOAD ${GTEST_ARCHIVE_URL} "${GTEST_ARCHIVE_DEST_PATH}"
		STATUS GTEST_DL_STATUS
		TLS_VERIFY ON
		EXPECTED_HASH SHA1=${GTEST_ARCHIVE_SHA1}
	)

	list(GET GTEST_DL_STATUS 0 GTEST_DL_STATUS_ERROR_CODE)
	list(GET GTEST_DL_STATUS 1 GTEST_DL_STATUS_ERROR_MGS)
	if (${GTEST_DL_STATUS_ERROR_CODE} EQUAL 0)
		status("Google Test successfully downloaded.")
	else()
		message(FATAL_ERROR "Error ${GTEST_DL_STATUS_ERROR_CODE} while \
			downloading Google Test: ${GTEST_DL_STATUS_ERROR_MGS}.")
	endif()
endif()

status("Extracting to '${GTEST_TMP_DIR}'")
execute_process(
	COMMAND ${CMAKE_COMMAND} -E tar xzf "${GTEST_ARCHIVE_DEST_PATH}"
	WORKING_DIRECTORY "${GTEST_TMP_DIR}"
)

set(GTEST_SRC_DIR "${GTEST_TMP_DIR}/googletest-${GTEST_VERSION}")
add_subdirectory("${GTEST_SRC_DIR}" ${GTEST_BIN_DIR})
mark_as_advanced(
	gtest_build_tests
	gtest_build_samples
)
