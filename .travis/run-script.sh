#!/bin/bash
set -ev

VARINT_COVERAGE="OFF" && [[ "${ENABLE_COVERAGE}" = true ]]  && VARINT_COVERAGE="ON"
VARINT_CONFIGURATION="Debug"

mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER="${CXX}" \
  -DCMAKE_C_COMPILER="${CC}" \
  -DCMAKE_INSTALL_PREFIX="../install" \
  -DCMAKE_BUILD_TYPE="${VARINT_CONFIGURATION}" \
  -DBUILD_GTEST=ON \
  -DVARINT_TESTS=ON \
  -DVARINT_COVERAGE=${VARINT_COVERAGE} ..
make all install
if [ "${ENABLE_COVERAGE}" = true ]; then
  make coverage # Implicitly runs tests
else
  ctest -C "${VARINT_CONFIGURATION}" --output-on-failure
fi
