module netcdf_cxx_get_variable_i_mod
    use iso_c_binding, only : c_int, c_ptr, c_float, c_long
    implicit none
    public

    interface

        function c_netcdfGetVarInt(&
                netcdfID, groupName, varName, data) &
                bind(C, name = "netcdfGetVarInt")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            integer(c_int), intent(out) :: data(:)
            integer(c_int) :: c_netcdfGetVarInt
        end function c_netcdfGetVarInt

        function c_netcdfGetVarInt64(&
                netcdfID, groupName, varName, data) &
                bind(C, name = "netcdfGetVarInt64")
            import :: c_int
            import :: c_long
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            integer(c_long), intent(out) :: data(:)
            integer(c_int) :: c_netcdfGetVarInt64
        end function c_netcdfGetVarInt64

        function c_netcdfGetVarReal(&
                netcdfID, groupName, varName, data) &
                bind(C, name = "netcdfGetVarReal")
            import :: c_int
            import :: c_float
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            real(c_float), intent(out) :: data(:)
            integer(c_int) :: c_netcdGetVarReal
        end function c_netcdfGetVarReal

        function c_netcdfGetVarString(&
                netcdfID, groupName, varName, data) &
                bind(C, name = "netcdfGetVarString")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), intent(out) :: data
            integer(c_int) :: c_netcdfGetVarString
        end function c_netcdfGetVarString

        function c_netcdfGetVarSize(&
                netcdfID, groupName, varName, varSize) &
                bind(C, name = "netcdfGetVarSize")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            integer(c_int), intent(out) :: varSize
            integer(c_int) :: c_netcdfGetVarSize
        end function c_netcdfGetVarSize

        subroutine c_netcdfFreeString(&
                numStrings, data) &
                bind(C, name = "netcdfFreeString")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: numStrings
            type(c_ptr), intent(inout) :: data
        end subroutine c_netcdfFreeString

    end interface

end module netcdf_cxx_get_variable_i_mod
