!> @brief Tests the creation and writing of an integer variable in a NetCDF file.
!>
!> This test creates a NetCDF file, defines a dimension, adds a group, and
!> defines an integer variable within that group. It then writes data to
!> the variable and verifies that all NetCDF operations complete successfully.
!>
!> The test performs the following steps:
!> 1. Create a NetCDF file.
!> 2. Define a dimension named "dim" with size 3.
!> 3. Add a group named "group".
!> 4. Define an integer variable named "int_var" within the group.
!> 5. Write the values [1, 2, 3] to the variable.
!> 6. Close the file and check for errors at each step.
subroutine int_variable_test()
    use test_utils_mod
    use netcdf_cxx_mod
    use netcdf
    implicit none
    integer :: status
    integer(c_int) :: netcdfID
    character(len=:), allocatable :: fileName
    character(len=:), allocatable :: varName
    character(len=:), allocatable :: groupName
    character(len=:), allocatable :: dimName
    integer(c_int) :: fileMode
    integer(c_int) :: result
    integer(c_int), allocatable :: data(:)

    fileName = "test_int_variable.nc"
    fileMode = 2
    varName = "int_var"
    groupName = "group"
    dimName = "dim"
    result = netcdfCreate(fileName, netcdfID, fileMode)
    call assertEqual(0, result, status, assert)
    result = netcdfAddDim(netcdfID, dimName, 3)
    call assertEqual(0, result, status, assert)
    result = netcdfAddGroup(netcdfID, groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfAddVar(netcdfID, varName, NF90_INT, 1, [dimName], groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfPutVar(netcdfID, varName, [1, 2, 3], groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfClose(netcdfID)
    call assertEqual(0, result, status, assert)

end subroutine int_variable_test

!> @brief Tests the creation and writing of a double variable in a NetCDF file.
!>
!> This test creates a NetCDF file, defines a dimension, adds a group, and
!> defines a double variable within that group. It then writes data to
!> the variable and verifies that all NetCDF operations complete successfully.
subroutine double_variable_test()
    use test_utils_mod
    use netcdf_cxx_mod
    use netcdf
    implicit none
    integer :: status
    integer(c_int) :: netcdfID
    character(len=:), allocatable :: fileName
    character(len=:), allocatable :: varName
    character(len=:), allocatable :: groupName
    character(len=:), allocatable :: dimName
    integer(c_int) :: fileMode
    integer(c_int) :: result
    real(c_double), allocatable :: data(:)

    fileName = "test_double_variable.nc"
    fileMode = 2
    varName = "double_var"
    groupName = "group"
    dimName = "dim"
    allocate(data(3))
      data = [1.111, 2.222, 3.333]
    result = netcdfCreate(fileName, netcdfID, fileMode)
    call assertEqual(0, result, status, assert)
    result = netcdfAddDim(netcdfID, dimName, 3)
    call assertEqual(0, result, status, assert)
    result = netcdfAddGroup(netcdfID, groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfAddVar(netcdfID, varName, NF90_DOUBLE, 1, [dimName], groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfPutVar(netcdfID, varName, data, groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfClose(netcdfID)
    call assertEqual(0, result, status, assert)

end subroutine double_variable_test

!> @brief Tests the creation, writing, and reading of a string variable in a NetCDF file.
!>
!> This test creates a NetCDF file, defines a dimension, adds a group, and
!> defines a string variable within that group. It writes an array of strings
!> to the variable, closes the file, reopens it, and verifies that the stored
!> values match the expected input.
!>
!> The test performs the following steps:
!> 1. Create a NetCDF file.
!> 2. Define a dimension named "dim" with size 3.
!> 3. Add a group named "group".
!> 4. Define a string variable named "string_var" within the group.
!> 5. Write the values ["one", "two", "three"] to the variable.
!> 6. Close the file and check for errors at each step.
!> 7. Reopen the file and read back the stored string values.
!> 8. Verify that the read values match the written values.
!> 9. Close the file and check for errors at each step.
subroutine string_variable_test()
    use test_utils_mod
    use netcdf_cxx_mod
    use netcdf
    use netcdf_cxx_get_variable_mod
    implicit none
    integer :: status
    integer(c_int) :: netcdfID
    character(len=:), allocatable :: fileName
    character(len=:), allocatable :: varName
    character(len=:), allocatable :: groupName
    character(len=:), allocatable :: dimName
    integer(c_int) :: fileMode
    integer(c_int) :: result
    integer :: i
    character(len=:), allocatable :: in_data(:)
    character(len=:), allocatable :: out_data(:)

    fileName = "test_string_variable.nc"
    fileMode = 2
    varName = "string_var"
    groupName = "group"
    dimName = "dim"


    result = netcdfCreate(fileName, netcdfID, fileMode)
    call assertEqual(0, result, status, assert)
    result = netcdfAddDim(netcdfID, dimName, 3)
    call assertEqual(0, result, status, assert)
    result = netcdfAddGroup(netcdfID, groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfAddVar(netcdfID, varName, NF90_STRING, 1, [dimName], groupName)
    call assertEqual(0, result, status, assert)
    in_data = ["one  ", "two  ", "three"]
    result = netcdfPutVar(netcdfID, varName, in_data, groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfClose(netcdfID)
    call assertEqual(0, result, status, assert)
    result = netcdfCreate(fileName, netcdfID, 0)
    result = netcdfGetVarString(netcdfID, varName, out_data, groupName)
    call assertEqual(0, result, status, assert)
    do i = 1, size(in_data)
        call assertEqual(trim(in_data(i)), trim(out_data(i)), status, assert)
    end do
    result = netcdfClose(netcdfID)
    call assertEqual(0, result, status, assert)

end subroutine string_variable_test


program netcdf_variable_test

    call int_variable_test()
    call double_variable_test()
    call string_variable_test()

end program netcdf_variable_test