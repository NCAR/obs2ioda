module Test_f_c_string_i_mod
    use iso_c_binding, only : c_ptr, c_int
    implicit none

    public :: to_upper
    public :: to_upper_array

    interface
        subroutine to_upper(c_string) bind(C, name = "to_upper")
            import :: c_ptr
            type(c_ptr), value, intent(in) :: c_string
        end subroutine to_upper

        subroutine to_upper_array(c_string_array, n) bind(C, name = "to_upper_array")
            import :: c_ptr, c_int
            type(c_ptr), value, intent(in) :: c_string_array
            integer(c_int), value :: n
        end subroutine to_upper_array

        subroutine variable_length_string_array(c_string_array, m) bind(C, name = "variable_length_string_array")
            import :: c_ptr, c_int
            type(c_ptr), intent(out) :: c_string_array
            integer(c_int), value :: m
        end subroutine variable_length_string_array

        subroutine free_variable_length_string_array(c_string_array, m) bind(C, name = "free_variable_length_string_array")
            import :: c_ptr, c_int
            type(c_ptr), intent(inout) :: c_string_array
            integer(c_int), value :: m
        end subroutine free_variable_length_string_array

    end interface

end module Test_f_c_string_i_mod
