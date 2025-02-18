module netcdf_cxx_mod
    use iso_c_binding, only : c_int, c_ptr, c_null_ptr, c_loc, c_float, c_long, c_f_pointer
    use f_c_string_t_mod, only : f_c_string_t
    use f_c_string_1D_t_mod, only : f_c_string_1D_t
    use netcdf, only : NF90_INT, NF90_REAL, NF90_STRING
    use netcdf_cxx_i_mod, only : c_netcdfCreate, c_netcdfClose, c_netcdfAddGroup, c_netcdfAddDim, &
            c_netcdfAddVar, c_netcdfPutVarInt, c_netcdfPutVarInt64, c_netcdfPutVarReal, c_netcdfPutVarString, &
            c_netcdfSetFillInt, c_netcdfSetFillInt64, c_netcdfSetFillReal, c_netcdfSetFillString, &
            c_netcdfGetVarInt, c_netcdfGetVarInt64, c_netcdfGetVarReal, c_netcdfGetVarString, &
            c_netcdfGetVarSize, c_netcdfFreeString, &
            c_netcdfPutAttInt, c_netcdfPutAttString
    implicit none
    public

    ! netcdfGetVar:
    !   Reads data from a variable in a NetCDF file.
    !
    !   Arguments:
    !     - netcdfID (integer(c_int), intent(in), value):
    !       The identifier of the NetCDF file from which data will be read.
    !     - varName (character(len=*), intent(in)):
    !       The name of the variable to be read.
    !     - data (class(*), allocatable, intent(out)):
    !       The output array where the read data will be stored.
    !       The type must match the variable's data type.
    !     - groupName (character(len=*), intent(in), optional):
    !       The name of the group containing the variable.
    !       If not provided, the variable is assumed to be a global variable.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating the outcome of the operation:
    !         - 0: Success.
    !         - Non-zero: Failure.
!    interface netcdfGetVar
!        module procedure netcdfGetVarInt
!        module procedure netcdfGetVarInt64
!        module procedure netcdfGetVarReal
!        module procedure netcdfGetVarString
!    end interface netcdfGetVar

contains

    ! netcdfCreate:
    !   Creates a new NetCDF file or opens an existing file in a specified mode,
    !   using a Fortran string for the file path.
    !
    !   Arguments:
    !     - path (character(len=*), intent(in)): The file path as a Fortran string.
    !     - netcdfID (integer(c_int), intent(inout)): On input, it may contain an
    !       identifier to be updated; on output, it holds the file identifier
    !       for the created or opened NetCDF file.
    !     - fileMode (integer(c_int), intent(in), optional):
    !         File mode for creating or opening the NetCDF file. Defaults to 2
    !         (replace mode). Possible values are:
    !           - 0: Open an existing file in read-only mode.
    !           - 1: Open an existing file for writing.
    !           - 2: Create a new file, overwriting any existing file.
    !           - 3: Create a new file, failing if the file already exists.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating success (0) or failure (non-zero).
    function netcdfCreate(path, netcdfID, fileMode)
        character(len = *), intent(in) :: path
        integer(c_int), intent(inout) :: netcdfID
        integer(c_int), intent(in), optional :: fileMode
        integer(c_int) :: netcdfCreate
        type(f_c_string_t) :: f_c_string_path
        type(c_ptr) :: c_path
        integer(c_int) :: mode
        ! Set the mode to the provided fileMode if present, otherwise default to 2
        if (present(fileMode)) then
            mode = fileMode
        else
            mode = 2
        end if
        c_path = f_c_string_path%to_c(path)
        netcdfCreate = c_netcdfCreate(c_path, netcdfID, mode)
    end function netcdfCreate

    ! netcdfClose:
    !   Closes a previously opened NetCDF file identified by its file identifier.
    !
    !   Arguments:
    !     - netcdfID (integer(c_int), intent(in), value): The identifier of the
    !       NetCDF file to close.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating success (0) or failure (non-zero).
    function netcdfClose(netcdfID)
        integer(c_int), value, intent(in) :: netcdfID
        integer(c_int) :: netcdfClose
        netcdfClose = c_netcdfClose(netcdfID)
    end function netcdfClose

    ! netcdfAddGroup:
    !   Adds a new group to a NetCDF file under a specified parent group.
    !
    !   Arguments:
    !     - netcdfID (integer(c_int), intent(in), value):
    !       The identifier of the NetCDF file where the group will be added.
    !     - groupName (character(len=*), intent(in)):
    !       The name of the new group to be created within the specified parent group.
    !     - parentGroupName (character(len=*), intent(in), optional):
    !       The name of the parent group under which the new group will be added.
    !       If not provided, the new group will be created in the root group.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating the outcome of the operation:
    !         - 0: Success.
    !         - Non-zero: Failure
    function netcdfAddGroup(netcdfID, groupName, parentGroupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in), optional :: parentGroupName
        character(len = *), intent(in) :: groupName
        integer(c_int) :: netcdfAddGroup
        type(c_ptr) :: c_parentGroupName
        type(c_ptr) :: c_groupName
        type(f_c_string_t) :: f_c_string_parentGroupName
        type(f_c_string_t) :: f_c_string_groupName

        if (present(parentGroupName)) then
            c_parentGroupName = f_c_string_parentGroupName%to_c(parentGroupName)
        else
            c_parentGroupName = c_null_ptr
        end if
        c_groupName = f_c_string_groupName%to_c(groupName)

        netcdfAddGroup = c_netcdfAddGroup(netcdfID, c_parentGroupName, c_groupName)
    end function netcdfAddGroup

    ! netcdfAddDim:
    !   Adds a new dimension to a NetCDF file, either in a specified group or as a global dimension.
    !
    ! Arguments:
    !   - netcdfID (integer(c_int), intent(in), value):
    !       Identifier of the NetCDF file.
    !   - dimName (character(len=*), intent(in)):
    !       Name of the new dimension.
    !   - len (integer(c_int), intent(in), value):
    !       Length of the dimension.
    !   - groupName (character(len=*), intent(in), optional):
    !       Name of the target group. If absent, the dimension is added as a global dimension.
    !
    ! Returns:
    !    - integer(c_int): A status code indicating the outcome of the operation:
    !       - 0: Success.
    !       - Non-zero: Failure
    function netcdfAddDim(netcdfID, dimName, len, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: dimName
        integer(c_int), value, intent(in) :: len
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfAddDim
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_dimName
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_dimName

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_dimName = f_c_string_dimName%to_c(dimName)

        netcdfAddDim = c_netcdfAddDim(netcdfID, c_groupName, c_dimName, len)

    end function netcdfAddDim

    ! netcdfAddVar:
    !   Adds a new variable to a NetCDF file, specifying its name, type, dimensions, and target group.
    !
    !   Arguments:
    !     - netcdfID (integer(c_int), intent(in), value):
    !       The identifier of the NetCDF file to which the variable will be added.
    !     - varName (character(len=*), intent(in)):
    !       The name of the new variable to be created.
    !     - netcdfDataType (integer(c_int), intent(in), value):
    !       The NetCDF data type of the variable (e.g., `NF90_INT`, `NF90_REAL`).
    !     - numDims (integer(c_int), intent(in), value):
    !       The number of dimensions associated with the variable.
    !     - dimNames (character(len=*), dimension(numDims), intent(in)):
    !       An array of dimension names that define the variable's shape.
    !     - groupName (character(len=*), intent(in), optional):
    !       The name of the group in which the variable will be created.
    !       If not provided, the variable will be added as a global variable.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating the outcome of the operation:
    !         - 0: Success.
    !         - Non-zero: Failure.
    function netcdfAddVar(netcdfID, varName, netcdfDataType, numDims, dimNames, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        integer(c_int), value, intent(in) :: netcdfDataType
        integer(c_int), value, intent(in) :: numDims
        character(len = *), dimension(numDims), intent(in) :: dimNames
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfAddVar
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(c_ptr) :: c_dimNames
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName
        type(f_c_string_1D_t) :: f_c_string_1D_dimNames

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)
        c_dimNames = f_c_string_1D_dimNames%to_c(dimNames)
        netcdfAddVar = c_netcdfAddVar(netcdfID, c_groupName, c_varName, &
                netcdfDataType, numDims, c_dimNames)
    end function netcdfAddVar

    ! netcdfPutVar:
    !   Writes data to a variable in a NetCDF file.
    !
    !   Arguments:
    !     - netcdfID (integer(c_int), intent(in), value):
    !       The identifier of the NetCDF file where the data will be written.
    !     - varName (character(len=*), intent(in)):
    !       The name of the variable to which data will be written.
    !     - values (class(*), dimension(:), intent(in)):
    !       The data to be written to the variable.
    !     - groupName (character(len=*), intent(in), optional):
    !       The name of the group containing the variable.
    !       If not provided, the variable is assumed to be a global variable.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating the outcome of the operation:
    !         -  0: Success.
    !         - -1: NetCDF operation returned an error, but the error code was 0.
    !         - -2: Unsupported type passed for values.
    !         - Other nonzero values: Specific NetCDF error codes.
    function netcdfPutVar(netcdfID, varName, values, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        class(*), dimension(:), intent(in) :: values
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfPutVar
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(c_ptr) :: c_values
        type(f_c_string_1D_t) :: f_c_string_1D_values

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)

        select type (values)
        type is (integer(c_int))
            c_values = c_loc(values)
            netcdfPutVar = c_netcdfPutVarInt(netcdfID, c_groupName, &
                    c_varName, c_values)

        type is (integer(c_long))
            c_values = c_loc(values)
            netcdfPutVar = c_netcdfPutVarInt64(netcdfID, c_groupName, &
                    c_varName, c_values)

        type is (real(c_float))
            c_values = c_loc(values)
            netcdfPutVar = c_netcdfPutVarReal(netcdfID, c_groupName, &
                    c_varName, c_values)

        type is (character(len = *))
            c_values = f_c_string_1D_values%to_c(values)
            netcdfPutVar = c_netcdfPutVarString(netcdfID, c_groupName, &
                    c_varName, c_values)
        class default
            netcdfPutVar = -2
        end select
    end function netcdfPutVar

    ! netcdfSetFill:
    !   Sets the fill mode and fill value for a variable in a NetCDF file.
    !
    !   Arguments:
    !     - netcdfID (integer(c_int), intent(in), value):
    !       The identifier of the NetCDF file containing the variable.
    !     - varName (character(len=*), intent(in)):
    !       The name of the variable for which the fill mode is set.
    !     - fillMode (integer(c_int), intent(in), value):
    !       The fill mode to be applied:
    !         - 0: Turn off fill mode (use uninitialized values).
    !         - 1: Turn on fill mode (use specified fill value).
    !     - fillValue (class(*), intent(in)):
    !       The fill value to be applied when fill mode is enabled.
    !       Must match the data type of the variable.
    !     - groupName (character(len=*), intent(in), optional):
    !       The name of the group containing the variable.
    !       If not provided, the variable is assumed to be a global variable.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating the outcome of the operation:
    !         -  0: Success.
    !         - -1: NetCDF operation returned an error, but the error code was 0.
    !         - -2: Unsupported type passed for fillValue.
    !         - Other nonzero values: Specific NetCDF error codes.
    function netcdfSetFill(netcdfID, varName, fillMode, fillValue, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        integer(c_int), value, intent(in) :: fillMode
        class(*), intent(in) :: fillValue
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfSetFill
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(f_c_string_t) :: f_c_string_fillValue

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)

        select type (fillValue)
        type is (integer(c_int))
            netcdfSetFill = c_netcdfSetFillInt(netcdfID, c_groupName, &
                    c_varName, fillMode, fillValue)

        type is (integer(c_long))
            netcdfSetFill = c_netcdfSetFillInt64(netcdfID, c_groupName, &
                    c_varName, fillMode, fillValue)

        type is (real(c_float))
            netcdfSetFill = c_netcdfSetFillReal(netcdfID, c_groupName, &
                    c_varName, fillMode, fillValue)

        type is (character(len = *))
            netcdfSetFill = c_netcdfSetFillString(netcdfID, c_groupName, &
                    c_varName, fillMode, &
                    f_c_string_fillValue%to_c(fillValue))
        class default
            netcdfSetFill = -2
        end select
    end function netcdfSetFill

    ! See interface netcdfGetVar documentation for details.
    function netcdfGetVarInt(netcdfID, varName, data, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        integer(c_int), allocatable, intent(out) :: data(:)
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfGetVarInt
        integer(c_int) :: varSize
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName
        type(c_ptr) :: c_data
        integer, pointer :: f_data_ptr(:)

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)
        netcdfGetVarInt = c_netcdfGetVarSize(netcdfID, c_groupName, c_varName, varSize)
        allocate(data(varSize))
        netcdfGetVarInt = c_netcdfGetVarInt(netcdfID, c_groupName, c_varName, c_data)
        call c_f_pointer(c_data, f_data_ptr, [varSize])
        allocate(data(varSize))
        data = f_data_ptr
    end function netcdfGetVarInt

    ! See interface netcdfGetVar documentation for details.
    function netcdfGetVarInt64(netcdfID, varName, data, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        integer(c_long), allocatable, intent(out) :: data(:)
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfGetVarInt64
        integer(c_int) :: varSize
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName
        type(c_ptr) :: c_data
        integer(c_long), pointer :: f_data_ptr(:)

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)
        netcdfGetVarInt64 = c_netcdfGetVarSize(netcdfID, c_groupName, c_varName, varSize)
        allocate(data(varSize))
        netcdfGetVarInt64 = c_netcdfGetVarInt64(netcdfID, c_groupName, c_varName, c_data)
        call c_f_pointer(c_data, f_data_ptr, [varSize])
        allocate(data(varSize))
        data = f_data_ptr
    end function netcdfGetVarInt64

    ! See interface netcdfGetVar documentation for details.
    function netcdfGetVarReal(netcdfID, varName, data, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        real(c_float), allocatable, intent(out) :: data(:)
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfGetVarReal
        integer(c_int) :: varSize
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName
        type(c_ptr) :: c_data
        real(c_float), pointer :: f_data_ptr(:)

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)
        netcdfGetVarReal = c_netcdfGetVarSize(netcdfID, c_groupName, c_varName, varSize)
        allocate(data(varSize))
        netcdfGetVarReal = c_netcdfGetVarReal(netcdfID, c_groupName, c_varName, c_data)
        call c_f_pointer(c_data, f_data_ptr, [varSize])
        allocate(data(varSize))
        data = f_data_ptr
    end function netcdfGetVarReal

    ! See interface netcdfGetVar documentation for details.
    function netcdfGetVarString(netcdfID, varName, data, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        character(len = :), allocatable, intent(out) :: data(:)
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfGetVarString
        integer(c_int) :: varSize
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName
        type(c_ptr) :: c_data
        type(f_c_string_1D_t) :: f_c_string_1D_data
        character(len = :), allocatable :: tmpData(:)

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        c_varName = f_c_string_varName%to_c(varName)
        netcdfGetVarString = c_netcdfGetVarSize(netcdfID, c_groupName, c_varName, varSize)
        allocate(character(len = 1) :: tmpData(varSize))
        c_data = f_c_string_1D_data%to_c(tmpData)
        netcdfGetVarString = c_netcdfGetVarString(netcdfID, c_groupName, c_varName, c_data)
        tmpData = f_c_string_1D_data%to_f(c_data, varSize)
        data = tmpData
        call c_netcdfFreeString(varSize, c_data)
    end function netcdfGetVarString

    ! netcdfPutAtt:
    !   Writes an attribute to a NetCDF variable, group, or as a global attribute.
    !
    !   Arguments:
    !     - netcdfID (integer(c_int), intent(in), value):
    !       The identifier of the NetCDF file.
    !     - attName (character(len=*), intent(in)):
    !       The name of the attribute to be written.
    !     - attValue (class(*), intent(in)):
    !       The value of the attribute. Must be of a supported NetCDF type,
    !       such as integer(c_int) or character(len=*). Unsupported types will
    !       result in an error with status code -2.
    !     - varName (character(len=*), intent(in), optional):
    !       The name of the variable to which the attribute will be assigned.
    !       If not provided, the attribute is assigned to the group instead.
    !     - groupName (character(len=*), intent(in), optional):
    !       The name of the group containing the variable.
    !       If not provided, the attribute is written as a global attribute.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating the outcome of the operation:
    !         -  0: Success.
    !         - -1: NetCDF operation returned an error, but the error code was 0.
    !         - -2: Unsupported type passed for attValue.
    !         - Other nonzero values: Specific NetCDF error codes.
    function netcdfPutAtt(netcdfID, attName, attValue, varName, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: attName
        class(*), intent(in) :: attValue
        character(len = *), optional, intent(in) :: varName
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfPutAtt
        type(f_c_string_t) :: f_c_string_attName
        type(f_c_string_t) :: f_c_string_groupName
        type(f_c_string_t) :: f_c_string_varName
        type(c_ptr) :: c_attName
        type(c_ptr) :: c_groupName
        type(c_ptr) :: c_varName
        type(c_ptr) :: c_attValue
        type(f_c_string_t) :: f_c_string_attValue

        if (present(groupName)) then
            c_groupName = f_c_string_groupName%to_c(groupName)
        else
            c_groupName = c_null_ptr
        end if
        if (present(varName)) then
            c_varName = f_c_string_varName%to_c(varName)
        else
            c_varName = c_null_ptr
        end if

        c_attName = f_c_string_attName%to_c(attName)

        select type (attValue)
        type is (integer(c_int))
            c_attValue = c_loc(attValue)
            netcdfPutAtt = c_netcdfPutAttInt(netcdfID, c_attName, c_attValue, c_varName, c_groupName)
        type is (character(len = *))
            c_attValue = f_c_string_attValue%to_c(attValue)
            netcdfPutAtt = c_netcdfPutAttString(netcdfID, c_attName, c_attValue, c_varName, c_groupName)
        class default
            netcdfPutAtt = -2
        end select
    end function netcdfPutAtt

end module netcdf_cxx_mod
