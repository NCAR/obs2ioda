program Test_netcdf_create
    use Test_netcdf_create_mod
    implicit none
    type(Test_netcdf_create_t) :: test
    call test%setUp()
    call test%test_netcdfCreate()
    call test%tearDown()
end program Test_netcdf_create