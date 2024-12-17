module Test_f_c_string_t_mod
    use fortran_test_framework_mod, only : assertEqual, assertTrue
    use f_c_string_t_mod, only : f_c_string_t
    use f_c_string_i_mod, only : strlen
    use Test_f_c_string_i_mod, only : to_upper
    use iso_c_binding, only : c_null_char, c_ptr
    implicit none
    contains
    subroutine Test_f_c_string_t()
        type(f_c_string_t) :: f_c_string1
        type(f_c_string_t) :: f_c_string2
        character(len=:), allocatable :: f_string
        character(len=:), allocatable :: f_string_upper
        character(len=:), allocatable :: f_string_ref
        character(len=:), allocatable :: f_string_upper_ref
        type(c_ptr) :: c_string
        integer :: c_string_len, n

        f_string_ref = "foo"
        f_string_upper_ref = "FOO"
        n = len(f_string_ref)

        c_string = f_c_string1%to_c2(f_string_ref)
        c_string_len = strlen(c_string)

        call assertEqual(len(f_string_ref), c_string_len)

        call to_upper(c_string)

        f_string_upper = f_c_string2%to_f2(c_string)

        call assertEqual(f_string_upper_ref, f_string_upper)

    end subroutine Test_f_c_string_t
end module Test_f_c_string_t_mod
