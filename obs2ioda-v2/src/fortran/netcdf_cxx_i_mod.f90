module netcdf_cxx_i_mod
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

        function c_netcdfPutAttInt(&
                netcdfID, groupName, varName, attName, data) &
                bind(C, name = "netcdfPutAttInt")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: attName
            type(c_ptr), value, intent(in) :: data
            integer(c_int) :: c_netcdfPutAttInt
        end function c_netcdfPutAttInt

        function c_netcdfPutAttInt1D(&
                netcdfID, groupName, varName, attName, data, len) &
                bind(C, name = "netcdfPutAttInt")
            import :: c_int
            import :: c_ptr
            import :: c_size_t
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: attName
            type(c_ptr), value, intent(in) :: data
            integer(c_size_t), value, intent(in) :: len
            integer(c_int) :: c_netcdfPutAttInt
        end function c_netcdfPutAttInt1D

        function c_netcdfPutAttString(&
                netcdfID, groupName, varName, attName, data) &
                bind(C, name = "netcdfPutAttString")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            type(c_ptr), value, intent(in) :: attName
            type(c_ptr), value, intent(in) :: data
            integer(c_int) :: c_netcdfPutAttString
        end function c_netcdfPutAttString

        function c_netcdfSetFillInt(&
                netcdfID, groupName, varName, fillMode, fillValue) &
                bind(C, name = "netcdfSetFillInt")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            integer(c_int), value, intent(in) :: fillMode
            integer(c_int), value, intent(in) :: fillValue
        end function c_netcdfSetFillInt

        function c_netcdfSetFillReal(&
                netcdfID, groupName, varName, fillMode, fillValue) &
                bind(C, name = "netcdfSetFillReal")
            import :: c_int
            import :: c_ptr
            import :: c_float
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            integer(c_int), value, intent(in) :: fillMode
            real(c_float), value, intent(in) :: fillValue
        end function c_netcdfSetFillReal

        function c_netcdfSetFillInt64(&
                netcdfID, groupName, varName, fillMode, fillValue) &
                bind(C, name = "netcdfSetFillInt64")
            import :: c_int
            import :: c_ptr
            import :: c_long
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            integer(c_int), value, intent(in) :: fillMode
            integer(c_long), value, intent(in) :: fillValue
        end function c_netcdfSetFillInt64

        function c_netcdfSetFillString(&
                netcdfID, groupName, varName, fillMode, fillValue) &
                bind(C, name = "netcdfSetFillString")
            import :: c_int
            import :: c_ptr
            integer(c_int), value, intent(in) :: netcdfID
            type(c_ptr), value, intent(in) :: groupName
            type(c_ptr), value, intent(in) :: varName
            integer(c_int), value, intent(in) :: fillMode
            type(c_ptr), value, intent(in) :: fillValue
        end function c_netcdfSetFillString
    end interface

end module netcdf_cxx_i_mod