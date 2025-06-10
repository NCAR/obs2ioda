module float_comparisons_mod
    use kinds, only: r_single, r_double
    implicit none
    private

    public :: equal, less_equal, less, greater_equal, greater

    ! equal:
    !   Returns true if two floating point values are approximately equal.
    !
    !   Arguments:
    !     - a (real(r_single or r_double), intent(in)):
    !       First value to compare.
    !     - b (real(r_single or r_double), intent(in)):
    !       Second value to compare.
    !     - atol (real(r_single or r_double), intent(in), optional):
    !       Absolute tolerance for equality check. If not provided, a default
    !       tolerance is used (1.0e-6 for single precision, 1.0e-8 for double precision).
    !
    !   Returns:
    !     - logical: True if |a - b| <= atol, false otherwise.
    interface equal
        module procedure equal_sp, equal_dp
    end interface

    ! less_equal:
    !   Returns true if the first value is less than or approximately equal to the second.
    !
    !   Arguments:
    !     - a (real(r_single or r_double), intent(in)):
    !       First value to compare.
    !     - b (real(r_single or r_double), intent(in)):
    !       Second value to compare.
    !     - atol (real(r_single or r_double), intent(in), optional):
    !       Absolute tolerance used to determine approximate equality.
    !
    !   Returns:
    !     - logical: True if a < b or a ≈ b within atol.
    interface less_equal
        module procedure less_equal_sp, less_equal_dp
    end interface

    ! less:
    !   Returns true if the first value is strictly less than the second,
    !   allowing for small differences around equality.
    !
    !   Arguments:
    !     - a (real(r_single or r_double), intent(in)):
    !       First value to compare.
    !     - b (real(r_single or r_double), intent(in)):
    !       Second value to compare.
    !     - atol (real(r_single or r_double), intent(in), optional):
    !       Absolute tolerance used to determine approximate equality.
    !
    !   Returns:
    !     - logical: True if a is strictly less than b (i.e., not greater than or approximately equal to b).
    interface less
        module procedure less_sp, less_dp
    end interface

    ! greater_equal:
    !   Returns true if the first value is greater than or approximately equal to the second.
    !
    !   Arguments:
    !     - a (real(r_single or r_double), intent(in)):
    !       First value to compare.
    !     - b (real(r_single or r_double), intent(in)):
    !       Second value to compare.
    !     - atol (real(r_single or r_double), intent(in), optional):
    !       Absolute tolerance used to determine approximate equality.
    !
    !   Returns:
    !     - logical: True if a > b or a ≈ b within atol.
    interface greater_equal
        module procedure greater_equal_sp, greater_equal_dp
    end interface

    ! greater:
    !   Returns true if the first value is strictly greater than the second,
    !   allowing for small differences around equality.
    !
    !   Arguments:
    !     - a (real(r_single or r_double), intent(in)):
    !       First value to compare.
    !     - b (real(r_single or r_double), intent(in)):
    !       Second value to compare.
    !     - atol (real(r_single or r_double), intent(in), optional):
    !       Absolute tolerance used to determine approximate equality.
    !
    !   Returns:
    !     - logical: True if a is strictly greater than b (i.e., not less than or approximately equal to b).
    interface greater
        module procedure greater_sp, greater_dp
    end interface

    ! Default absolute tolerances
    real(r_single), parameter :: default_atol_sp = 1.0e-6_r_single
    real(r_double), parameter :: default_atol_dp = 1.0e-8_r_double
contains
    !======================
    ! Single precision
    !======================

    ! See documentation for equal
    logical function equal_sp(a, b, atol)
        real(r_single), intent(in) :: a, b
        real(r_single), intent(in), optional :: atol
        real(r_single) :: abs_tol
        if (.not. present(atol)) then
            abs_tol = default_atol_sp
        else
            abs_tol = atol
        end if
        equal_sp = abs(a - b) <= abs_tol
    end function equal_sp

    ! See documentation for less_equal
    logical function less_equal_sp(a, b, atol)
        real(r_single), intent(in) :: a, b
        real(r_single), intent(in), optional :: atol
        less_equal_sp = (a < b) .or. equal_sp(a, b, atol)
    end function less_equal_sp

    ! See documentation for less
    logical function less_sp(a, b, atol)
        real(r_single), intent(in) :: a, b
        real(r_single), intent(in), optional :: atol
        less_sp = .not. greater_equal_sp(a, b, atol)
    end function less_sp

    ! See documentation for greater_equal
    logical function greater_equal_sp(a, b, atol)
        real(r_single), intent(in) :: a, b
        real(r_single), intent(in), optional :: atol
        greater_equal_sp = (a > b) .or. equal_sp(a, b, atol)
    end function greater_equal_sp

    ! See documentation for greater
    logical function greater_sp(a, b, atol)
        real(r_single), intent(in) :: a, b
        real(r_single), intent(in), optional :: atol
        greater_sp = .not. less_equal_sp(a, b, atol)
    end function greater_sp

    !======================
    ! Double precision
    !======================

    ! See documentation for equal
    logical function equal_dp(a, b, atol)
        real(r_double), intent(in) :: a, b
        real(r_double), intent(in), optional :: atol
        real(r_double) :: abs_tol

        if (.not. present(atol)) then
            abs_tol = default_atol_sp
        else
            abs_tol = atol
        end if
        equal_dp = abs(a - b) <= abs_tol
    end function equal_dp

    ! See documentation for less_equal
    logical function less_equal_dp(a, b, atol)
        real(r_double), intent(in) :: a, b
        real(r_double), intent(in), optional :: atol
        less_equal_dp = (a < b) .or. equal_dp(a, b, atol)
    end function less_equal_dp

    ! See documentation for less
    logical function less_dp(a, b, atol)
        real(r_double), intent(in) :: a, b
        real(r_double), intent(in), optional :: atol
        less_dp = .not. greater_equal_dp(a, b, atol)
    end function less_dp

    ! See documentation for greater_equal
    logical function greater_equal_dp(a, b, atol)
        real(r_double), intent(in) :: a, b
        real(r_double), intent(in), optional :: atol
        greater_equal_dp = (a > b) .or. equal_dp(a, b, atol)
    end function greater_equal_dp

    ! See documentation for greater
    logical function greater_dp(a, b, atol)
        real(r_double), intent(in) :: a, b
        real(r_double), intent(in), optional :: atol
        greater_dp = .not. less_equal_dp(a, b, atol)
    end function greater_dp

end module float_comparisons_mod
