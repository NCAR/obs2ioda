module Test_f_c_string_t_mod
    use fortran_test_framework_mod, only : assertEqual, assert
    use f_c_string_t_mod, only : f_c_string_t, c_strlen
    use Test_f_c_string_i_mod, only : to_upper
    use iso_c_binding, only : c_null_char, c_ptr, c_null_ptr
    implicit none
    contains
    subroutine Test_f_c_string_t()
        integer, dimension(1)  :: dimids
        integer :: varid
        integer :: nctype = 12
        type(f_c_string_t), allocatable :: f_c_string1
        type(f_c_string_t), allocatable :: f_c_string2
        type(f_c_string_t), allocatable :: f_c_string_null

        type(f_c_string_t), allocatable :: f_c_string_empty
        character(len=:), allocatable :: f_string
        character(len=:), allocatable :: f_string_upper
        character(len=:), allocatable :: f_string_ref
        character(len=:), allocatable :: f_string_upper_ref
        character(len=:), allocatable :: f_string_variable_length
        character(len=:), allocatable :: f_string_null

        type(c_ptr) :: c_string
        integer :: c_string_len, f_string_len, status

        status = 0
        f_string_ref = "foo"
        f_string_upper_ref = "FOO"
        f_string_len = len(f_string_ref)

        allocate(f_c_string1)
        allocate(f_c_string2)
        allocate(f_c_string_empty)

        c_string = f_c_string_empty%to_c("")
        c_string_len= c_strlen(c_string)
        call assertEqual(0, c_string_len, status, assert)

        c_string = f_c_string1%to_c(f_string_ref)
        c_string_len = c_strlen(c_string)
        call assertEqual(f_string_len, c_string_len, status, assert)

        call to_upper(c_string)
        f_string_upper = f_c_string2%to_f(c_string)
        call assertEqual(f_string_upper_ref, f_string_upper, status, assert)
        c_string_len = c_strlen(c_null_ptr)
        call assertEqual(-1, c_string_len, status, assert)

    end subroutine Test_f_c_string_t
end module Test_f_c_string_t_mod
