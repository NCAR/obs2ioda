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


end module netcdf_cxx_mod