
subroutine create_netcdf_file_replace_mode_test()
    use test_utils_mod
    use netcdf_cxx_mod
    implicit none
    integer :: status
    integer(c_int) :: netcdfID
    character(len=:), allocatable :: path
    integer(c_int) :: fileMode
    integer(c_int) :: result

    ! Create a NetCDF file in replace mode
    path = "test.nc"
    fileMode = 2
    result = netcdfCreate(path, netcdfID, fileMode)
    call assertEqual(0, result, status, assert)

    ! Create a NetCDF file in replace mode again. This should fail
    ! as the netcdfID is already in the file map.
    result = netcdfCreate(path, netcdfID, fileMode)
    call assertEqual(13, result, status, assert)

!    ! Close the file. This should remove the netcdfID from the file map.
    result = netcdfClose(netcdfID)
    call assertEqual(0, result, status, assert)
!
!    ! Create a NetCDF file in replace mode again. This should succeed as the netcdfID was removed from the file map.
    result = netcdfCreate(path, netcdfID)
    call assertEqual(0, result, status, assert)
!    ! Close the file.
    result = netcdfClose(netcdfID)
    call assertEqual(0, result, status, assert)
end subroutine create_netcdf_file_replace_mode_test

subroutine create_netcdf_file_new_file_mode_test()
    use test_utils_mod
    use netcdf_cxx_mod
    implicit none
    integer :: status
    integer(c_int) :: netcdfID
    character(len=:), allocatable :: path
    integer(c_int) :: fileMode
    integer(c_int) :: result

    ! Create a NetCDF file in new file mode
    path = "test.nc"
    fileMode = 3
    ! Should fail with -35 (file already exists) error code.
    result = netcdfCreate(path, netcdfID, fileMode)
    call assertEqual(-35, result, status, assert)

    ! This should also fail as the file was never created.
    result = netcdfCreate(path, netcdfID, fileMode)
    call assertEqual(-35, result, status, assert)
end subroutine create_netcdf_file_new_file_mode_test

program netcdf_file_test

    call create_netcdf_file_replace_mode_test()
    call create_netcdf_file_new_file_mode_test()

end program netcdf_file_test