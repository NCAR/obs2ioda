module Test_f_c_string_t_mod
    use fortran_test_framework_mod, only : assertEqual, assertTrue
    use f_c_string_t_mod, only : f_c_string_t
    use iso_c_binding, only : c_null_char, c_ptr
    implicit none
    contains
    subroutine Test_to_c_fc_string()
        type(f_c_string_t) :: f_c_string1
        type(f_c_string_t) :: f_c_string2
        character(len=:), allocatable :: f_string
        character(len=:), allocatable :: f_string_control
        character(len=:), allocatable :: f_string_upper
        type(c_ptr) :: c_string

        f_string_control = "foo"
        f_c_string1%f_string = f_string_control

        call f_c_string1%to_c1()
        c_string = f_c_string2%to_c2(f_string_control)

        call assertEqual(len(trim(f_string_control))+1, len(trim(f_c_string1%fc_string)))
        call assertEqual(len(trim(f_string_control))+1, len(trim(f_c_string2%fc_string)))

        f_c_string1%f_string = ""

        call f_c_string1%to_f1()
        f_string = f_c_string2%to_f2(c_string)

        call assertEqual(f_string, f_string_control)
        call assertEqual(f_c_string1%f_string, f_string_control)


    end subroutine Test_to_c_fc_string
end module Test_f_c_string_t_mod
