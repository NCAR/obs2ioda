module Test_f_c_string_t_mod
    use fortran_test_framework_mod, only : assertEqual, assertTrue
    use f_c_string_t_mod, only : f_c_string_t
    use iso_c_binding, only : c_null_char
    implicit none
    contains
    subroutine Test_to_c_fc_string()
        type(f_c_string_t) :: f_c_string
        character(len=:), allocatable :: f_string
        f_string = "foo"
        f_c_string%f_string = f_string
        call f_c_string%to_c()
        call assertEqual(len(trim(f_string))+1, len(trim(f_c_string%fc_string)))
        call f_c_string%cleanup()
        call assertTrue(.not.allocated(f_c_string%fc_string))
    end subroutine Test_to_c_fc_string
end module Test_f_c_string_t_mod
