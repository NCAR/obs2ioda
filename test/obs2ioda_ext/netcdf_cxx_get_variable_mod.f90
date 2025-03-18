module netcdf_cxx_get_variable_mod
    use iso_c_binding, only : c_int, c_ptr, c_null_ptr, c_loc, c_float, c_long, c_f_pointer
    use f_c_string_t_mod, only : f_c_string_t
    use f_c_string_1D_t_mod, only : f_c_string_1D_t
    use netcdf, only : NF90_INT, NF90_REAL, NF90_STRING
    use netcdf_cxx_i_mod, only : c_netcdfCreate, c_netcdfClose, c_netcdfAddGroup, c_netcdfAddDim, &
            c_netcdfAddVar, c_netcdfPutVarInt, c_netcdfPutVarInt64, c_netcdfPutVarReal, c_netcdfPutVarString, &
            c_netcdfSetFillInt, c_netcdfSetFillInt64, c_netcdfSetFillReal, c_netcdfSetFillString, &
            c_netcdfPutAttInt, c_netcdfPutAttString
    use netcdf_cxx_get_variable_i_mod, only : c_netcdfGetVarSize, c_netcdfGetVarInt, c_netcdfGetVarInt64, &
            c_netcdfGetVarReal, c_netcdfGetVarString, c_netcdfFreeString
    implicit none
    public

    ! netcdfGetVar:
    !   Reads values from a variable in a NetCDF file.
    !
    !   Arguments:
    !     - netcdfID (integer(c_int), intent(in), value):
    !       The identifier of the NetCDF file from which values will be read.
    !     - varName (character(len=*), intent(in)):
    !       The name of the variable to be read.
    !     - values (class(*), allocatable, intent(out)):
    !       The output array where the read values will be stored.
    !       The type must match the variable's values type.
    !     - groupName (character(len=*), intent(in), optional):
    !       The name of the group containing the variable.
    !       If not provided, the variable is assumed to be a global variable.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating the outcome of the operation:
    !         - 0: Success.
    !         - Non-zero: Failure.
    interface netcdfGetVar
        module procedure netcdfGetVarInt
        module procedure netcdfGetVarInt64
        module procedure netcdfGetVarReal
        module procedure netcdfGetVarString
    end interface netcdfGetVar

contains

    ! See interface netcdfGetVar documentation for details.
    function netcdfGetVarInt(netcdfID, varName, values, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        integer(c_int), allocatable, intent(out) :: values(:)
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfGetVarInt
        integer(c_int) :: varSize
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)
        netcdfGetVarInt = c_netcdfGetVarSize(netcdfID, c_groupName, c_varName, varSize)
        if (allocated(values)) deallocate(values)
        allocate(values(varSize))
        values(:) = 0
        netcdfGetVarInt = c_netcdfGetVarInt(netcdfID, c_groupName, c_varName, values)
    end function netcdfGetVarInt

    ! See interface netcdfGetVar documentation for details.
    function netcdfGetVarInt64(netcdfID, varName, values, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        integer(c_long), allocatable, target, intent(out) :: values(:)
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfGetVarInt64
        integer(c_int) :: varSize
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)
        netcdfGetVarInt64 = c_netcdfGetVarSize(netcdfID, c_groupName, c_varName, varSize)
        if (allocated(values)) deallocate(values)
        allocate(values(varSize))
        netcdfGetVarInt64 = c_netcdfGetVarInt64(netcdfID, c_groupName, c_varName, values)
    end function netcdfGetVarInt64

    ! See interface netcdfGetVar documentation for details.
    function netcdfGetVarReal(netcdfID, varName, values, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        real(c_float), allocatable, target, intent(out) :: values(:)
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfGetVarReal
        integer(c_int) :: varSize
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)
        netcdfGetVarReal = c_netcdfGetVarSize(netcdfID, c_groupName, c_varName, varSize)
        if (allocated(values)) deallocate(values)
        allocate(values(varSize))
        netcdfGetVarReal = c_netcdfGetVarReal(netcdfID, c_groupName, c_varName, values)
    end function netcdfGetVarReal

    ! See interface netcdfGetVar documentation for details.
    function netcdfGetVarString(netcdfID, varName, values, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        character(len = :), allocatable, intent(out) :: values(:)
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfGetVarString
        integer(c_int) :: varSize
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName
        type(c_ptr) :: c_values
        type(f_c_string_1D_t) :: f_c_string_1D_values
        character(len = :), allocatable :: tmpData(:)

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)
        netcdfGetVarString = c_netcdfGetVarSize(netcdfID, c_groupName, c_varName, varSize)
        netcdfGetVarString = c_netcdfGetVarString(netcdfID, c_groupName, c_varName, c_values)
        tmpData = f_c_string_1D_values%to_f(c_values, varSize)
        values = tmpData
        call c_netcdfFreeString(varSize, c_values)
    end function netcdfGetVarString

end module netcdf_cxx_get_variable_mod
