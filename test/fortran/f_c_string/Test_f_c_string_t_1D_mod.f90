module Test_f_c_string_1D_mod
    use f_c_string_1D_t_mod, only : f_c_string_1D_t
    use Test_f_c_string_i_mod, only : to_upper_array, variable_length_string_array, free_variable_length_string_array
    use fortran_test_framework_mod, only : assertEqual, assert
    use iso_c_binding, only : c_ptr, c_null_ptr, c_null_char
    implicit none

contains

    subroutine invoke_f_c_string_1D_final(f_c_string_1D)
        type(f_c_string_1D_t), intent(out) :: f_c_string_1D
    end subroutine invoke_f_c_string_1D_final

    subroutine Test_f_c_string_t_1D()
        type(f_c_string_1D_t) :: f_c_string_1D_1
        type(f_c_string_1D_t) :: f_c_string_1D_2
        type(f_c_string_1D_t) :: f_c_string_1D_3
        type(f_c_string_1D_t) :: f_c_string_1D_null
        character(len = :), dimension(:), allocatable :: f_string_1D
        character(len = :), dimension(:), allocatable :: f_string_1D_null
        character(len = :), dimension(:), allocatable :: f_string_1D_variable_length
        character(len = :), dimension(:), allocatable :: f_string_1D_upper
        character(len = :), dimension(:), allocatable :: f_string_1D_ref
        character(len = :), dimension(:), allocatable :: f_string_1D_upper_ref
        character(len = :), dimension(:), allocatable :: f_string_1D_variable_length_ref
        type(c_ptr) :: c_string_1D
        type(c_ptr) :: c_string_1D_variable_length
        integer :: i, m, status

        status = 0
        m = 5
        f_string_1D_ref = ["abc", "def", "ghi", "jkl", "mno"]
        f_string_1D_variable_length_ref = ["a    ", "aa   ", "aaa  ", "aaaa ", "aaaaa"]
        f_string_1D_upper_ref = ["ABC", "DEF", "GHI", "JKL", "MNO"]

        c_string_1D = f_c_string_1D_1%to_c(f_string_1D_ref)

        call to_upper_array(c_string_1D, m)

        f_string_1D_upper = f_c_string_1D_2%to_f(c_string_1D, m)
        do i = 1, m
            call assertEqual(f_string_1D_upper_ref(i), &
                    f_string_1D_upper(i), status, assert)
        end do

        f_string_1D_upper = f_c_string_1D_2%to_f(c_string_1D, m)
        !Test if allocation logic is correct in to_f
        do i = 1, m
            call assertEqual(f_string_1D_upper_ref(i), &
                    f_string_1D_upper(i), status, assert)
        end do

        call assertEqual(.true., allocated(f_c_string_1D_1%fc_string_1D), status, assert)
        ! Tests if the final subroutine is called
        call invoke_f_c_string_1D_final(f_c_string_1D_1)
        call assertEqual(.false., allocated(f_c_string_1D_1%fc_string_1D), status, assert)

        call variable_length_string_array(c_string_1D_variable_length, m)
        f_string_1D_variable_length = f_c_string_1D_3%to_f(c_string_1D_variable_length, m)
        do i = 1, m
            call assertEqual(f_string_1D_variable_length_ref(i), &
                    f_string_1D_variable_length(i), status, assert)
        end do
        f_string_1D_null = f_c_string_1D_null%to_f(c_null_ptr, 1)
        call assertEqual("", f_string_1D_null(1), status, assert)
        call free_variable_length_string_array(c_string_1D_variable_length, m)

    end subroutine Test_f_c_string_t_1D

end module Test_f_c_string_1D_mod
