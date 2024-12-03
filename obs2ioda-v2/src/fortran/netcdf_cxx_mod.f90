module netcdf_cxx_mod
    use iso_c_binding, only : c_char, c_null_char, c_null_ptr, c_int
    use f_c_string_t_mod, only : f_c_string_t
    use f_c_string_1D_t_mod, only : f_c_string_1D_t
    use netcdf_cxx_i_mod
    use netcdf, only : NF90_INT, NF90_INT64, NF90_REAL
    implicit none
    public

contains
    ! Helper to handle optional groupName initialization
    subroutine init_optional_string(input_string, c_string)
        character(len = *), intent(in), optional :: input_string
        type(f_c_string_t), intent(inout) :: c_string
        if (present(input_string)) then
            c_string%f_string = input_string
            call c_string%to_c()
        else
            c_string%c_string = c_null_ptr
        end if
    end subroutine init_optional_string

    function netcdfCreate(path, netcdfID)
        character(len = *), intent(in) :: path
        integer(c_int), intent(inout) :: netcdfID
        integer(c_int) :: netcdfCreate
        type(f_c_string_t) :: c_path

        c_path%f_string = path
        call c_path%to_c()
        netcdfCreate = c_netcdfCreate(c_path%c_string, netcdfID)
        call c_path%cleanup()
    end function netcdfCreate

    function netcdfClose(netcdfID)
        integer(c_int), value, intent(in) :: netcdfID
        integer(c_int) :: netcdfClose
        netcdfClose = c_netcdfClose(netcdfID)
    end function netcdfClose

    function netcdfAddGroup(netcdfID, groupName, parentGroupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in), optional :: parentGroupName
        character(len = *), intent(in) :: groupName
        integer(c_int) :: netcdfAddGroup
        type(f_c_string_t) :: c_parentGroupName, c_groupName

        call init_optional_string(parentGroupName, c_parentGroupName)
        c_groupName%f_string = groupName
        call c_groupName%to_c()

        netcdfAddGroup = c_netcdfAddGroup(netcdfID, c_parentGroupName%c_string, c_groupName%c_string)
        call c_parentGroupName%cleanup()
        call c_groupName%cleanup()
    end function netcdfAddGroup

    function netcdfAddDim(netcdfID, dimName, len, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: dimName
        integer(c_int), value, intent(in) :: len
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfAddDim
        type(f_c_string_t) :: c_groupName, c_dimName

        call init_optional_string(groupName, c_groupName)
        c_dimName%f_string = dimName
        call c_dimName%to_c()

        netcdfAddDim = c_netcdfAddDim(netcdfID, c_groupName%c_string, c_dimName%c_string, len)

        call c_groupName%cleanup()
        call c_dimName%cleanup()
    end function netcdfAddDim

    function netcdfAddVar(netcdfID, varName, netcdfDataType, numDims, dimNames, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        integer(c_int), value, intent(in) :: netcdfDataType
        integer(c_int), value, intent(in) :: numDims
        character(len = *), dimension(numDims), intent(in) :: dimNames
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfAddVar
        type(f_c_string_t) :: c_groupName, c_varName
        type(f_c_string_1D_t) :: c_dimNames

        call init_optional_string(groupName, c_groupName)
        c_varName%f_string = varName
        call c_varName%to_c()
        c_dimNames%f_string_1D = dimNames
        call c_dimNames%to_c()

        netcdfAddVar = c_netcdfAddVar(netcdfID, c_groupName%c_string, c_varName%c_string, netcdfDataType, numDims, c_dimNames%c_string_1D)

        call c_groupName%cleanup()
        call c_varName%cleanup()
        call c_dimNames%cleanup()
    end function netcdfAddVar

    function netcdfPutVar(netcdfID, varName, data, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        class(*), dimension(:), intent(in) :: data
        character(len = *), optional, intent(in) :: groupName
        integer(c_int) :: netcdfPutVar
        type(f_c_string_t) :: c_groupName
        type(f_c_string_t) :: c_varName
        type(c_ptr) :: c_data
        type(f_c_string_1D_t) :: c_string_data

        ! Helper to init the optional groupName
        call init_optional_string(groupName, c_groupName)

        ! Convert variable name to C-compatible string
        c_varName%f_string = varName
        call c_varName%to_c()

        select type (data)
        type is (integer(c_int))
            c_data = c_loc(data)
            netcdfPutVar = c_netcdfPutVarInt(netcdfID, c_groupName%c_string, &
                    c_varName%c_string, c_data)

        type is (integer(c_long))
            c_data = c_loc(data)
            netcdfPutVar = c_netcdfPutVarInt64(netcdfID, c_groupName%c_string, &
                    c_varName%c_string, c_data)

        type is (real(c_float))
            c_data = c_loc(data)
            netcdfPutVar = c_netcdfPutVarReal(netcdfID, c_groupName%c_string, &
                    c_varName%c_string, c_data)

        type is (character(len = *))
            c_string_data%f_string_1D = data
            call c_string_data%to_c()
            netcdfPutVar = c_netcdfPutVarString(netcdfID, c_groupName%c_string, &
                    c_varName%c_string, c_string_data%c_string_1D)
            call c_string_data%cleanup()
        end select

        ! Cleanup allocated strings
        call c_groupName%cleanup()
        call c_varName%cleanup()
    end function netcdfPutVar

    function netcdfPutAtt(&
            netcdfID, attName, data, varName, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: attName
        class(*), intent(in) :: data
        character(len = *), intent(in), optional :: varName
        character(len = *), intent(in), optional :: groupName
        integer(c_int) :: netcdfPutAtt

        type(f_c_string_t) :: c_attName
        type(f_c_string_t) :: c_varName
        type(f_c_string_t) :: c_groupName
        type(c_ptr) :: c_data
        type(f_c_string_t) :: c_string_data

        c_attName%f_string = attName
        call c_attName%to_c()
        call init_optional_string(varName, c_varName)
        call init_optional_string(groupName, c_groupName)
        select type (data)
        type is (integer(c_int))
            c_data = c_loc(data)
            netcdfPutAtt = c_netcdfPutAttInt(netcdfID, c_groupName%c_string, &
                    c_varName%c_string, c_attName%c_string, c_data)
        type is (character(len = *))
            c_string_data%f_string = data
            call c_string_data%to_c()
            netcdfPutAtt = c_netcdfPutAttString(netcdfID, c_groupName%c_string, &
                    c_varName%c_string, c_attName%c_string, c_string_data%c_string)
            call c_string_data%cleanup()
        end select
        call c_attName%cleanup()
        call c_varName%cleanup()
        call c_groupName%cleanup()
    end function netcdfPutAtt

    function netcdfSetFill(netcdfID, varName, fillMode, fillValue, groupName)
        integer(c_int), value, intent(in) :: netcdfID
        character(len = *), intent(in) :: varName
        integer(c_int), value, intent(in) :: fillMode
        class(*), intent(in) :: fillValue
        character(len = *), intent(in), optional :: groupName
        integer(c_int) :: netcdfSetFill
        type(f_c_string_t) :: c_groupName
        type(f_c_string_t) :: c_varName
        type(f_c_string_t) :: c_string_data

        ! Helper to init the optional groupName
        call init_optional_string(groupName, c_groupName)

        ! Convert variable name to C-compatible string
        c_varName%f_string = varName
        call c_varName%to_c()
        select type (fillValue)
        type is (integer(c_int))
            netcdfSetFill = c_netcdfSetFillInt(netcdfID, c_groupName%c_string, &
                    c_varName%c_string, fillMode, fillValue)

        type is (integer(c_long))
            netcdfSetFill = c_netcdfSetFillInt64(netcdfID, c_groupName%c_string, &
                    c_varName%c_string, fillMode, fillValue)

        type is (real(c_float))
            netcdfSetFill = c_netcdfSetFillReal(netcdfID, c_groupName%c_string, &
                    c_varName%c_string, fillMode, fillValue)
        type is (character(len = *))
            netcdfSetFill = c_netcdfSetFillString(netcdfID, c_groupName%c_string, &
                    c_varName%c_string, fillMode, c_null_ptr)
        end select

        ! Cleanup allocated strings
        call c_groupName%cleanup()
        call c_varName%cleanup()
    end function netcdfSetFill


end module netcdf_cxx_mod