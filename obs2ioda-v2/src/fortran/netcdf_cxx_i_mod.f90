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
    end interface

end module netcdf_cxx_i_mod