#!/bin/bash
set -ev

if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
  # CMake
  mkdir -p "${CMAKE_ROOT_DIR}"
  wget https://cmake.org/files/v3.2/cmake-3.2.3-Linux-x86_64.tar.gz -O "${CMAKE_ROOT_DIR}/cmake.tar.gz"
  tar xf "${CMAKE_ROOT_DIR}/cmake.tar.gz" -C "${CMAKE_ROOT_DIR}" --strip-components 1

  if [ "${ENABLE_COVERAGE}" = true ]; then
    # LCOV
    mkdir -p "${LCOV_ROOT_DIR}"
    wget https://github.com/linux-test-project/lcov/releases/download/v1.13/lcov-1.13.tar.gz -O "${LCOV_ROOT_DIR}/lcov.tar.gz"
    tar xf "${LCOV_ROOT_DIR}/lcov.tar.gz" -C "${LCOV_ROOT_DIR}" --strip-components 1

    # Coveralls-LCOV
    gem install coveralls-lcov
  fi
fi
