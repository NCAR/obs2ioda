module netcdf_c_i_mod
    use iso_c_binding
    implicit none
    private

    public :: netcdfCreate
    public :: netcdfClose
    public :: netcdfAddGroup
    public :: netcdfAddDim
    public :: netcdfPutAttInt
    public :: netcdfAddVar
    public :: netcdfPutVarString1D
    public :: netcdfPutVarInt1D
    public :: netcdfPutVarInt641D
    public :: netcdfPutVarReal1D
    public :: netcdfGetVarInt1D
    public :: netcdfGetVarInt641D
    public :: netcdfGetVarReal1D
    public :: netcdfGetVarString1D

    interface
        function netcdfCreate(path, ncid) bind(C, name = "netcdfCreate")
            import :: c_char, c_int, c_ptr
            type(c_ptr), value :: path
            integer(c_int), intent(out) :: ncid
            integer(c_int) :: netcdfCreate
        end function

        function netcdfAddGroup(ncid, parentGroupName, groupName) bind(C, name = "netcdfAddGroup")
            import :: c_char, c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: parentGroupName
            type(c_ptr), value :: groupName
            integer(c_int) :: netcdfAddGroup
        end function

        function netcdfAddDim(ncid, groupName, dimName, len) bind(C, name = "netcdfAddDim")
            import :: c_char, c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value :: dimName
            integer(c_int), value, intent(in) :: len
            integer(c_int) :: netcdfAddDim
        end function

        function netcdfPutAttInt(ncid, groupName, varName, attName, value) bind(C, name = "netcdfPutAttInt")
            import :: c_char, c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value :: attName
            integer(c_int), value, intent(in) :: value
            integer(c_int) :: netcdfPutAttr
        end function

        function netcdfAddVar(ncid, groupName, varName, netcdfDataType, numDims, dimNames) bind(C, name = "netcdfAddVar")
            import :: c_char, c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value :: varName
            integer(c_int), value, intent(in) :: netcdfDataType
            integer(c_int), value, intent(in) :: numDims
            type(c_ptr), value, intent(in) :: dimNames
            integer(c_int) :: netcdfAddVar
        end function

        function netcdfPutVarString1D(ncid, groupName, varName, data) bind(C, name = "netcdfPutVarString1D")
            import :: c_char, c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: data
            integer(c_int) :: netcdfPutVarString1D
        end function

        function netcdfPutVarInt1D(ncid, groupName, varName, data) bind(C, name = "netcdfPutVarInt1D")
            import :: c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: data
            integer(c_int) :: netcdfPutVarInt1D
        end function

        function netcdfPutVarInt641D(ncid, groupName, varName, data) bind(C, name = "netcdfPutVarInt641D")
            import :: c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: data
            integer(c_int) :: netcdfPutVarInt641D
        end function

        function netcdfPutVarReal1D(ncid, groupName, varName, data) bind(C, name = "netcdfPutVarReal1D")
            import :: c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: data
            integer(c_int) :: netcdfPutVarReal1D
        end function

        function netcdfGetVarInt1D(ncid, groupName, varName, data) bind(C, name = "netcdfGetVarInt1D")
            import :: c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), intent(out) :: data
            integer(c_int) :: netcdfGetVarInt1D
        end function

        function netcdfGetVarInt641D(ncid, groupName, varName, data) bind(C, name = "netcdfGetVarInt641D")
            import :: c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), intent(out) :: data
            integer(c_int) :: netcdfGetVarInt641D
        end function

        function netcdfGetVarReal1D(ncid, groupName, varName, data) bind(C, name = "netcdfGetVarReal1D")
            import :: c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), intent(out) :: data
            integer(c_int) :: netcdfGetVarReal1D
        end function

        function netcdfGetVarString1D(ncid, groupName, varName, data) bind(C, name = "netcdfGetVarString1D")
            import :: c_int, c_ptr
            integer(c_int), value, intent(in) :: ncid
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), intent(out) :: data
            integer(c_int) :: netcdfGetVarString1D
        end function

        function netcdfClose(ncid) bind(C, name = "netcdfClose")
            import :: c_int
            integer(c_int), value, intent(in) :: ncid
            integer(c_int) :: netcdfClose
        end function
    end interface
end module netcdf_c_i_mod