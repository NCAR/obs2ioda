get_target_property(
        GTEST_INCLUDE_DIRECTORIES GTest::gtest_main INTERFACE_INCLUDE_DIRECTORIES
)

set(NETCDF_TEST_SOURCES
        ${CMAKE_SOURCE_DIR}/test/cxx/netcdf_test_fixture.cc
        ${CMAKE_SOURCE_DIR}/test/cxx/netcdf_test.cc
)
set(NETCDF_TEST_INCLUDE_DIRS
        ${CMAKE_SOURCE_DIR}/test/cxx
        ${CMAKE_SOURCE_DIR}/obs2ioda-v2/src/cxx
        ${GTEST_INCLUDE_DIRECTORIES}
)
set(NETCDF_TEST_LIBRARY_DEPENDENCIES
        obs2ioda_cxx GTest::gtest_main
)

add_cxx_ctest(NetcdfTest "${NETCDF_TEST_SOURCES}"
        "${NETCDF_TEST_INCLUDE_DIRS}"
        "${NETCDF_TEST_LIBRARY_DEPENDENCIES}"
)
add_memcheck_ctest(NetcdfTest)