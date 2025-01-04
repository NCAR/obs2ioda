module Test_f_c_string_1D_mod
    ! Provides unit tests for the `f_c_string_1D_t` type.
    !
    ! Tests include conversion between Fortran and C strings, and
    ! string manipulations such as converting to uppercase.

    use f_c_string_1D_t_mod, only : f_c_string_1D_t
    use Test_f_c_string_i_mod, only : to_upper_array, variable_length_string_array, free_variable_length_string_array
    use fortran_test_framework_mod, only : assertEqual, assert
    use iso_c_binding, only : c_ptr, c_null_ptr, c_null_char
    implicit none

    type Test_f_c_string_1D_t
        ! Contains reference data and expected results for testing.
        !
        ! Fields:
        ! - f_string_1D_ref_1: Reference array of original strings (set 1).
        ! - f_string_1D_upper_ref_1: Expected uppercase version of strings (set 1).
        ! - f_string_1D_ref_2: Reference array of original strings (set 2).
        ! - f_string_1D_upper_ref_2: Expected uppercase version of strings (set 2).
        ! - m: Number of strings in each test array.

        character(len = 3), dimension(3) :: f_string_1D_ref_1 = &
                ["abc", "def", "ghi"]
        character(len = 3), dimension(3) :: f_string_1D_upper_ref_1 = &
                ["ABC", "DEF", "GHI"]
        character(len = 3), dimension(3) :: f_string_1D_ref_2 = &
                ["ghi", "def", "abc"]
        character(len = 3), dimension(3) :: f_string_1D_upper_ref_2 = &
                ["GHI", "DEF", "ABC"]
        integer :: m = 3

    contains

        procedure :: f_c_string_t_1D_single_string_single_instance => &
                f_c_string_t_1D_single_string_single_instance
        ! Tests the conversion and manipulation of a single string array
        ! for one instance of `f_c_string_1D_t`.
        !
        ! Arguments:
        ! - this: Test instance containing reference and expected data.

        procedure :: f_c_string_t_1D_single_string_double_instance => &
                f_c_string_t_1D_single_string_double_instance
        ! Tests conversion of a single string array between two
        ! instances of `f_c_string_1D_t`.
        !
        ! Arguments:
        ! - this: Test instance containing reference and expected data.

        procedure :: f_c_string_t_1D_double_string_single_instance => &
                f_c_string_t_1D_double_string_single_instance
        ! Tests the conversion and manipulation of two string arrays
        ! for one instance of `f_c_string_1D_t`.
        !
        ! Arguments:
        ! - this: Test instance containing reference and expected data.

        procedure :: f_c_string_t_1D_double_string_double_instance_mixed => &
                f_c_string_t_1D_double_string_double_instance_mixed
        ! Tests conversion of two string arrays between two
        ! mixed instances of `f_c_string_1D_t`.
        !
        ! Arguments:
        ! - this: Test instance containing reference and expected data.

    end type Test_f_c_string_1D_t

contains

    subroutine invoke_f_c_string_1D_final(f_c_string_1D)
        ! Finalizes a `f_c_string_1D_t` instance.
        !
        ! Arguments:
        ! - f_c_string_1D: The `f_c_string_1D_t` instance to finalize.
        type(f_c_string_1D_t), intent(out) :: f_c_string_1D
    end subroutine invoke_f_c_string_1D_final

    subroutine f_c_string_t_1D_single_string_single_instance(this)
        ! Tests single string array conversion and manipulation
        ! for one instance of `f_c_string_1D_t`.
        !
        ! Arguments:
        ! - this: Test instance containing reference and expected data.
        class(Test_f_c_string_1D_t), intent(inout) :: this
        type(f_c_string_1D_t) :: f_c_string_1D
        character(len = :), dimension(:), allocatable :: f_string_1D_upper
        type(c_ptr) :: c_string_1D
        integer :: i, status

        c_string_1D = f_c_string_1D%to_c(this%f_string_1D_ref_1)
        call to_upper_array(c_string_1D, this%m)
        f_string_1D_upper = f_c_string_1D%to_f(c_string_1D, this%m)
        do i = 1, this%m
            call assertEqual(f_string_1D_upper(i), this%f_string_1D_upper_ref_1(i), status, assert)
        end do
    end subroutine f_c_string_t_1D_single_string_single_instance

    subroutine f_c_string_t_1D_single_string_double_instance(this)
        ! Tests single string array conversion between two instances
        ! of `f_c_string_1D_t`.
        !
        ! Arguments:
        ! - this: Test instance containing reference and expected data.
        class(Test_f_c_string_1D_t), intent(inout) :: this
        type(f_c_string_1D_t) :: f_c_string_1D_1, f_c_string_1D_2
        character(len = :), dimension(:), allocatable :: f_string_1D_upper
        type(c_ptr) :: c_string_1D
        integer :: i, status

        c_string_1D = f_c_string_1D_1%to_c(this%f_string_1D_ref_1)
        call to_upper_array(c_string_1D, this%m)
        f_string_1D_upper = f_c_string_1D_2%to_f(c_string_1D, this%m)
        do i = 1, this%m
            call assertEqual(f_string_1D_upper(i), this%f_string_1D_upper_ref_1(i), status, assert)
        end do
    end subroutine f_c_string_t_1D_single_string_double_instance

    subroutine f_c_string_t_1D_double_string_single_instance(this)
        ! Tests double string array conversion for a single instance
        ! of `f_c_string_1D_t`.
        !
        ! Arguments:
        ! - this: Test instance containing reference and expected data.
        class(Test_f_c_string_1D_t), intent(inout) :: this
        type(f_c_string_1D_t) :: f_c_string_1D
        character(len = :), dimension(:), allocatable :: f_string_1D_upper_1, f_string_1D_upper_2
        type(c_ptr) :: c_string_1D_1, c_string_1D_2
        integer :: i, status

        c_string_1D_1 = f_c_string_1D%to_c(this%f_string_1D_ref_1)
        c_string_1D_2 = f_c_string_1D%to_c(this%f_string_1D_ref_2)
        call to_upper_array(c_string_1D_1, this%m)
        call to_upper_array(c_string_1D_2, this%m)
        f_string_1D_upper_1 = f_c_string_1D%to_f(c_string_1D_1, this%m)
        f_string_1D_upper_2 = f_c_string_1D%to_f(c_string_1D_2, this%m)
        do i = 1, this%m
            call assertEqual(f_string_1D_upper_1(i), this%f_string_1D_upper_ref_1(i), status, assert)
            call assertEqual(f_string_1D_upper_2(i), this%f_string_1D_upper_ref_2(i), status, assert)
        end do
    end subroutine f_c_string_t_1D_double_string_single_instance

    subroutine f_c_string_t_1D_double_string_double_instance_mixed(this)
        ! Tests double string array conversion between two mixed instances
        ! of `f_c_string_1D_t`.
        !
        ! Arguments:
        ! - this: Test instance containing reference and expected data.
        class(Test_f_c_string_1D_t), intent(inout) :: this
        type(f_c_string_1D_t) :: f_c_string_1D_1, f_c_string_1D_2
        character(len = :), dimension(:), allocatable :: f_string_1D_upper_1, f_string_1D_upper_2
        type(c_ptr) :: c_string_1D_1, c_string_1D_2
        integer :: i, status

        c_string_1D_1 = f_c_string_1D_1%to_c(this%f_string_1D_ref_1)
        c_string_1D_2 = f_c_string_1D_2%to_c(this%f_string_1D_ref_2)
        call to_upper_array(c_string_1D_1, this%m)
        call to_upper_array(c_string_1D_2, this%m)
        f_string_1D_upper_1 = f_c_string_1D_2%to_f(c_string_1D_1, this%m)
        f_string_1D_upper_2 = f_c_string_1D_1%to_f(c_string_1D_2, this%m)
        do i = 1, this%m
            call assertEqual(f_string_1D_upper_1(i), this%f_string_1D_upper_ref_1(i), status, assert)
            call assertEqual(f_string_1D_upper_2(i), this%f_string_1D_upper_ref_2(i), status, assert)
        end do
    end subroutine f_c_string_t_1D_double_string_double_instance_mixed

end module Test_f_c_string_1D_mod
