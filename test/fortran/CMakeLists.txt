add_pfunit_ctest(Test_netcdf_mod
  TEST_SOURCES Test_netcdf_mod.pf Test_netcdf_fixture_t_mod.pf
  LINK_LIBRARIES v2
  )
add_memcheck_ctest(Test_netcdf_mod)