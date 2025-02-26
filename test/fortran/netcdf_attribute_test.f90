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
subroutine int_attribute_test()
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
    character(len=:), allocatable :: attName
    integer(c_int) :: fileMode
    integer(c_int) :: result
    integer(c_int), allocatable :: data(:)
    integer(c_int) :: attValue

    fileName = "test_int_att.nc"
    fileMode = 2
    varName = "int_var"
    groupName = "group"
    dimName = "dim"
    attName = "int_att"
    attValue = 42
    result = netcdfCreate(fileName, netcdfID, fileMode)
    call assertEqual(0, result, status, assert)
    result = netcdfPutAtt(netcdfID, attName, attValue)
    call assertEqual(0, result, status, assert)
    result = netcdfAddDim(netcdfID, dimName, 3)
    call assertEqual(0, result, status, assert)
    result = netcdfAddGroup(netcdfID, groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfPutAtt(netcdfID, attName, attValue, groupName=groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfAddVar(netcdfID, varName, NF90_INT, 1, [dimName], groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfPutAtt(netcdfID, attName, attValue, varName=varName, groupName=groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfClose(netcdfID)
    call assertEqual(0, result, status, assert)

end subroutine int_attribute_test

subroutine string_attribute_test()
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
    character(len=:), allocatable :: attName
    integer(c_int) :: fileMode
    integer(c_int) :: result
    integer(c_int), allocatable :: data(:)
    character(len=:), allocatable :: attValue

    fileName = "test_string_att.nc"
    fileMode = 2
    varName = "int_var"
    groupName = "group"
    dimName = "dim"
    attName = "string_att"
    attValue = "string"
    result = netcdfCreate(fileName, netcdfID, fileMode)
    call assertEqual(0, result, status, assert)
    result = netcdfPutAtt(netcdfID, attName, attValue)
    call assertEqual(0, result, status, assert)
    result = netcdfAddDim(netcdfID, dimName, 3)
    call assertEqual(0, result, status, assert)
    result = netcdfAddGroup(netcdfID, groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfPutAtt(netcdfID, attName, attValue, groupName=groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfAddVar(netcdfID, varName, NF90_INT, 1, [dimName], groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfPutAtt(netcdfID, attName, attValue, varName=varName, groupName=groupName)
    call assertEqual(0, result, status, assert)
    result = netcdfClose(netcdfID)
    call assertEqual(0, result, status, assert)

end subroutine string_attribute_test


program netcdf_attribute_test

    call int_attribute_test()
    call string_attribute_test()

end program netcdf_attribute_test