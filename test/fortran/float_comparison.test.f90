! test_float_comparisons.f90
!-----------------------------------------------
! Module and driver for floating-point comparison tests
!
! test_float_comparisons_mod:
!   Defines test cases for comparison routines (equal, less, less_equal,
!   greater, greater_equal) in both single-precision and double-precision.
!
!   Test Parameters:
!     - a_sp (real(r_single), parameter): Single-precision reference value (1.0).
!     - b_sp (real(r_single), parameter): Single-precision test value (1.0 + 1.0e-7).
!     - a_dp (real(r_double), parameter): Double-precision reference value (1.0).
!     - b_dp (real(r_double), parameter): Double-precision test value (1.0 + 1.0d-9).
!
module test_float_comparisons_mod
    use kinds, only : r_single, r_double
    use float_comparisons_mod, only : equal, less, less_equal, greater, greater_equal
    implicit none
    real(r_single), parameter :: a_sp = 1.0_r_single
    real(r_double), parameter :: a_dp = 1.0_r_double
    real(r_single), parameter :: b_sp = 1.0_r_single + 1.0e-7
    real(r_double), parameter :: b_dp = 1.0_r_double + 1.0d-9

contains

    ! equal_sp_1:
    !   Verifies that equal(a_sp, b_sp) returns true using the algorithm's default tolerance.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None (stops with status 1 on failure, prints success message otherwise).
    subroutine equal_sp_1()
        logical :: result
        result = equal(a_sp, b_sp)
        if (.not. result) then
            print *, "Test failed: equal_sp 1"
            stop 1
        else
            print *, "Test passed: equal_sp 1"
        end if
    end subroutine equal_sp_1

    ! equal_sp_2:
    !   Verifies that equal(a_sp, b_sp) returns false when using a tighter tolerance of 1.0e-8.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine equal_sp_2()
        logical :: result
        result = .not. equal(a_sp, b_sp, atol = 1.0e-8_r_single)
        if (result) then
            print *, "Test passed: equal_sp 2 (tighter tolerance)"
        else
            print *, "Test failed: equal_sp 2 (tighter tolerance)"
            stop 1
        end if
    end subroutine equal_sp_2

    ! less_equal_sp_1:
    !   Verifies that less_equal(a_sp, b_sp) returns true with default tolerance.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine less_equal_sp_1()
        logical :: result
        result = less_equal(a_sp, b_sp)
        if (.not. result) then
            print *, "Test failed: less_equal_sp 1"
            stop 1
        else
            print *, "Test passed: less_equal_sp 1"
        end if
    end subroutine less_equal_sp_1

    ! less_sp_1:
    !   Verifies that less(a_sp, b_sp) returns true using a tolerance of 1.0e-8.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine less_sp_1()
        logical :: result
        result = less(a_sp, b_sp, atol = 1.0e-8_r_single)
        if (.not. result) then
            print *, "Test failed: less_sp 1"
            stop 1
        else
            print *, "Test passed: less_sp 1"
        end if
    end subroutine less_sp_1

    ! greater_equal_sp_1:
    !   Verifies that greater_equal(a_sp, b_sp) returns true with default tolerance.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine greater_equal_sp_1()
        logical :: result
        result = greater_equal(a_sp, b_sp)
        if (result) then
            print *, "Test passed: greater_equal_sp 1"
        else
            print *, "Test failed: greater_equal_sp 1"
            stop 1
        end if
    end subroutine greater_equal_sp_1

    ! greater_sp_1:
    !   Verifies that greater(a_sp + 1.0e-5, b_sp) returns true using a tolerance of 1.0e-8.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine greater_sp_1()
        logical :: result
        result = greater(a_sp + 1.0e-5_r_single, b_sp, atol = 1.0e-8_r_single)
        if (result) then
            print *, "Test passed: greater_sp 1"
        else
            print *, "Test failed: greater_sp 1"
            stop 1
        end if
    end subroutine greater_sp_1

    ! equal_dp_1:
    !   Verifies that equal(a_dp, b_dp) returns true with default tolerance.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine equal_dp_1()
        logical :: result
        result = equal(a_dp, b_dp)
        if (.not. result) then
            print *, "Test failed: equal_dp 1"
            stop 1
        else
            print *, "Test passed: equal_dp 1"
        end if
    end subroutine equal_dp_1

    ! equal_dp_2:
    !   Verifies that equal(a_dp, b_dp) returns false when using a tighter tolerance of 1.0e-10.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine equal_dp_2()
        logical :: result
        result = .not. equal(a_dp, b_dp, atol = 1.0e-10_r_double)
        if (result) then
            print *, "Test passed: equal_dp 2 (tighter tolerance)"
        else
            print *, "Test failed: equal_dp 2 (tighter tolerance)"
            stop 1
        end if
    end subroutine equal_dp_2

    ! less_equal_dp_1:\n  !   Verifies that less_equal(a_dp, b_dp) returns true with default tolerance.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine less_equal_dp_1()
        logical :: result
        result = less_equal(a_dp, b_dp)
        if (.not. result) then
            print *, "Test failed: less_equal_dp 1"
            stop 1
        else
            print *, "Test passed: less_equal_dp 1"
        end if
    end subroutine less_equal_dp_1

    ! less_dp_1:
    !   Verifies that less(a_dp, b_dp) returns true using a tolerance of 1.0e-10.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine less_dp_1()
        logical :: result
        result = less(a_dp, b_dp, atol = 1.0e-10_r_double)
        if (.not. result) then
            print *, "Test failed: less_dp 1"
            stop 1
        else
            print *, "Test passed: less_dp 1"
        end if
    end subroutine less_dp_1

    ! greater_equal_dp_1:
    !   Verifies that greater_equal(b_dp, a_dp) returns true with default tolerance.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine greater_equal_dp_1()
        logical :: result
        result = greater_equal(b_dp, a_dp)
        if (.not. result) then
            print *, "Test failed: greater_equal_dp 1"
            stop 1
        else
            print *, "Test passed: greater_equal_dp 1"
        end if
    end subroutine greater_equal_dp_1

    ! greater_dp_1:
    !   Verifies that greater(a_dp + 1.0e-5, b_dp) returns true using a tolerance of 1.0e-10.
    !
    !   Arguments:
    !     - None
    !
    !   Returns:
    !     - None
    subroutine greater_dp_1()
        logical :: result
        result = greater(a_dp + 1.0e-5_r_double, b_dp, atol = 1.0e-10_r_double)
        if (.not. result) then
            print *, "Test failed: greater_dp 1"
            stop 1
        else
            print *, "Test passed: greater_dp 1"
        end if
    end subroutine greater_dp_1

end module test_float_comparisons_mod

! test_float_comparisons:
!   Driver program to execute all floating-point comparison tests.
!
program test_float_comparisons
    use test_float_comparisons_mod
    implicit none

    ! Run single-precision tests
    call equal_sp_1()
    call equal_sp_2()
    call less_equal_sp_1()
    call less_sp_1()
    call greater_equal_sp_1()
    call greater_sp_1()

    ! Run double-precision tests
    call equal_dp_1()
    call equal_dp_2()
    call less_equal_dp_1()
    call less_dp_1()
    call greater_equal_dp_1()
    call greater_dp_1()

    print *, "All float comparison tests passed."
end program test_float_comparisons
