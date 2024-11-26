module f_c_numeric_mod
    use iso_c_binding, only : c_ptr, c_loc, c_f_pointer
    use kinds, only : r_double, r_kind, i_kind, i_long, i_llong

contains
    function to_c_double_1D(f_double_1D)
        real(r_double), target, intent(in) :: f_double_1D(:)
        type(c_ptr) :: to_c_double_1D
        to_c_double_1D = c_loc(f_double_1D)
    end function to_c_double_1D

    function to_c_real_1D(f_real_1D)
        real(r_kind), target, intent(in) :: f_real_1D(:)
        type(c_ptr) :: to_c_real_1D
        to_c_real_1D = c_loc(f_real_1D)
    end function to_c_real_1D

    function to_c_int_1D(f_int_1D)
        integer(i_kind), target, intent(in) :: f_int_1D(:)
        type(c_ptr) :: to_c_int_1D
        to_c_int_1D = c_loc(f_int_1D)
    end function to_c_int_1D

    function to_c_int64_1D(f_int64_1D)
        integer(i_llong), target, intent(in) :: f_int64_1D(:)
        type(c_ptr) :: to_c_int64_1D
        to_c_int64_1D = c_loc(f_int64_1D)
    end function to_c_int64_1D

    function to_f_real_1D(c_real_1D, n)
        type(c_ptr), intent(in) :: c_real_1D
        integer, intent(in) :: n
        real(r_kind), pointer :: to_f_real_1D(:)
        call c_f_pointer(c_real_1D, to_f_real_1D, [n])
    end function to_f_real_1D

    function to_f_double_1D(c_double_1D, n)
        type(c_ptr), intent(in) :: c_double_1D
        integer, intent(in) :: n
        real(r_double), pointer :: to_f_double_1D(:)
        call c_f_pointer(c_double_1D, to_f_double_1D, [n])
    end function to_f_double_1D

    function to_f_int_1D(c_int_1D, n)
        type(c_ptr), intent(in) :: c_int_1D
        integer, intent(in) :: n
        integer(i_kind), pointer :: to_f_int_1D(:)
        call c_f_pointer(c_int_1D, to_f_int_1D, [n])
    end function to_f_int_1D

    function to_f_int64_1D(c_int64_1D, n)
        type(c_ptr), intent(in) :: c_int64_1D
        integer, intent(in) :: n
        integer(i_llong), pointer :: to_f_int64_1D(:)
        call c_f_pointer(c_int64_1D, to_f_int64_1D, [n])
    end function to_f_int64_1D

end module f_c_numeric_mod