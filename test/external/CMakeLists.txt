################################################################################
#
# varint tests external dependencies
#
# Author: Stephane Poirier (stephane.poirier01@gmail.com)
#
################################################################################

project(varint-tests-external VERSION 0.1)

status("Importing Google Test")

set(GTEST_VERSION "release-1.7.0")
set(GTEST_ARCHIVE_URL "https://github.com/google/googletest/archive/${GTEST_VERSION}.zip")
set(GTEST_ARCHIVE_SHA1 "f89bc9f55477df2fde082481e2d709bfafdb057b")

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
		SHOW_PROGRESS
		TLS_VERIFY ON
		URL_HASH SHA1=${GTEST_ARCHIVE_SHA1}
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
