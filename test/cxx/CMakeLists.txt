include_directories(${GTEST_INCLUDE_DIRS} )
set(NETCDF_TEST_SOURCES
    ${CMAKE_SOURCE_DIR}/test/cxx/netcdf_test_fixture.cc
    ${CMAKE_SOURCE_DIR}/test/cxx/netcdf_test.cc
        )

set(NETCDF_SATWIND_TEST_SOURCES
        netcdf_satwind_test.cc
        )
list(TRANSFORM NETCDF_SATWIND_TEST_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
add_executable(
        NetcdfTest
        ${NETCDF_TEST_SOURCES}
)
target_include_directories(
        NetcdfTest PUBLIC ${CMAKE_SOURCE_DIR}/obs2ioda-v2/src/cxx
)
target_link_libraries(
        NetcdfTest PUBLIC gtest_main NetCDF::NetCDF_CXX NetCDF::NetCDF_C netcdf_c
)

add_test(
        NAME Netcdf_test
        COMMAND NetcdfTest --gtest_filter=*
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/cxx)
add_memcheck_ctest(NetcdfTest)

add_executable(
        NetcdfSatwindTest
        ${NETCDF_SATWIND_TEST_SOURCES}
)
target_include_directories(
        NetcdfSatwindTest PUBLIC ${CMAKE_SOURCE_DIR}/obs2ioda-v2/src/cxx
)
target_link_libraries(
        NetcdfSatwindTest PUBLIC gtest_main NetCDF::NetCDF_CXX NetCDF::NetCDF_C netcdf_c
)
add_test(
        NAME Netcdf_satwind_test
        COMMAND NetcdfSatwindTest --gtest_filter=*
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/cxx)
add_memcheck_ctest(NetcdfSatwindTest)

