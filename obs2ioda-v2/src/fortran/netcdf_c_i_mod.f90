module netcdf_c_i_mod
    use iso_c_binding
    implicit none
    public

    interface
        function c_netcdfCreate(path, netcdfID) &
                bind(C, name = "netcdfCreate")
            import :: c_int
            import :: c_ptr
            type(c_ptr), value, intent(in) :: path
            integer(c_int), intent(out) :: netcdfID
            integer(c_int) :: c_netcdfCreate
        end function

        function c_netcdfClose(netcdfID) &
                bind(C, name = "netcdfClose")
            import :: c_int
            integer(c_int), value, intent(in) :: netcdfID
            integer(c_int) :: c_netcdfClose
        end function

        function c_netcdfAddGroup(&
                netcdfID, parentGroupName, groupName) &
                bind(C, name = "netcdfAddGroup")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: parentGroupName
            type(c_ptr), value, intent(in) :: groupName
            integer(c_int) :: c_netcdfAddGroup
        end function c_netcdfAddGroup

        function c_netcdfAddDim(&
                netcdfID, groupName, dimName, len) &
                bind(C, name = "netcdfAddDim")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: dimName
            integer(c_int), value, intent(in) :: len
            integer(c_int) :: c_netcdfAddDim
        end function c_netcdfAddDim

        function c_netcdfAddVar(&
                netcdfID, groupName, varName, netcdfDataType, numDims, dimNames) &
                bind(C, name = "netcdfAddVar")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            integer(c_int), value, intent(in) :: netcdfDataType
            integer(c_int), value, intent(in) :: numDims
            type(c_ptr), value, intent(in) :: dimNames
            integer(c_int) :: c_netcdfAddVar
        end function c_netcdfAddVar

        function c_netcdfPutVarInt(&
                netcdfID, groupName, varName, data) &
                bind(C, name = "netcdfPutVarInt")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: data
            integer(c_int) :: c_netcdfPutVarInt
        end function c_netcdfPutVarInt

        function c_netcdfPutVarInt64(&
                netcdfID, groupName, varName, data) &
                bind(C, name = "netcdfPutVarInt64")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: data
            integer(c_int) :: c_netcdfPutVarInt64
        end function c_netcdfPutVarInt64


        function c_netcdfPutVarReal(&
                netcdfID, groupName, varName, data) &
                bind(C, name = "netcdfPutVarReal")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: data
            integer(c_int) :: c_netcdfPutVarReal
        end function c_netcdfPutVarReal

        function c_netcdfPutVarString(&
                netcdfID, groupName, varName, data) &
                bind(C, name = "netcdfPutVarString")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: data
            integer(c_int) :: c_netcdfPutVarString
        end function c_netcdfPutVarString
    end interface

end module netcdf_c_i_mod