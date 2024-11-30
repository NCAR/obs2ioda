add_pfunit_ctest(Test_netcdf_create_mod
  TEST_SOURCES Test_netcdf_create_mod.pf Test_netcdf_fixture_t_mod.pf
  LINK_LIBRARIES v2
  )
add_memcheck_ctest(Test_netcdf_create_mod)

add_pfunit_ctest(Test_netcdf_add_group_mod
  TEST_SOURCES Test_netcdf_add_group_mod.pf Test_netcdf_fixture_t_mod.pf
  LINK_LIBRARIES v2
  )
add_memcheck_ctest(Test_netcdf_add_group_mod)

add_pfunit_ctest(Test_netcdf_add_dim_mod
  TEST_SOURCES Test_netcdf_add_dim_mod.pf Test_netcdf_fixture_t_mod.pf
  LINK_LIBRARIES v2
  )
add_memcheck_ctest(Test_netcdf_add_dim_mod)

add_pfunit_ctest(Test_netcdf_var_mod
  TEST_SOURCES Test_netcdf_var_mod.pf Test_netcdf_fixture_t_mod.pf
  LINK_LIBRARIES v2
  )
add_memcheck_ctest(Test_netcdf_var_mod)

add_pfunit_ctest(Test_netcdf_att_mod
                 TEST_SOURCES Test_netcdf_att_mod.pf Test_netcdf_fixture_t_mod.pf
                 LINK_LIBRARIES v2
)
add_memcheck_ctest(Test_netcdf_att_mod)
