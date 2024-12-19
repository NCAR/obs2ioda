module Test_fortran_test_framework_mod
    use fortran_test_framework_mod, only : assertEqual, assert
    implicit none

contains
    ! Subroutine: assert_mock
    ! A mock assertion handler used for testing the behavior of `assertEqual`.
    !
    ! Arguments:
    ! - condition (logical, in): The condition to evaluate.
    ! - message (character, in): The message associated with the assertion (not used in this mock).
    ! - status (integer, out): The status code set based on the condition (0 for success, 1 for failure).
    !
    ! Notes:
    ! - Unlike `assert`, this subroutine does not terminate the program on failure.
    ! - Useful for testing assertion logic without stopping the test suite.
    subroutine assert_mock(condition, message, status)
        logical, intent(in) :: condition
        character(len = *), intent(in) :: message
        integer, intent(out) :: status

        if (.not. condition) then
            status = 1
        else
            status = 0
        end if
    end subroutine assert_mock

    ! Subroutine: test_assertEqual_integer
    ! Tests the `assertEqual` subroutine with integer values.
    !
    ! Behavior:
    ! - Asserts equality of integers using both the mock assertion handler (`assert_mock`)
    !   and the default `assert` procedure.
    ! - Verifies correct handling of equal and unequal integer values.
    subroutine test_assertEqual_integer()
        integer :: status
        call assertEqual(1, 1, status, assert_mock)
        call assertEqual(0, status, status, assert)

        call assertEqual(1, 2, status, assert_mock)
        call assertEqual(1, status, status, assert)
    end subroutine test_assertEqual_integer

    ! Subroutine: test_assertEqual_logical
    ! Tests the `assertEqual` subroutine with logical values.
    !
    ! Behavior:
    ! - Asserts equality of logical values (`.true.` and `.false.`) using both the mock
    !   assertion handler (`assert_mock`) and the default `assert` procedure.
    ! - Verifies correct handling of equal and unequal logical values.
    subroutine test_assertEqual_logical()
        integer :: status
        call assertEqual(.true., .true., status, assert_mock)
        call assertEqual(0, status, status, assert)

        call assertEqual(.true., .false., status, assert_mock)
        call assertEqual(1, status, status, assert)
    end subroutine test_assertEqual_logical

    ! Subroutine: test_assertEqual_string
    ! Tests the `assertEqual` subroutine with string values.
    !
    ! Behavior:
    ! - Asserts equality of strings using both the mock assertion handler (`assert_mock`)
    !   and the default `assert` procedure.
    ! - Verifies correct handling of equal and unequal string values.
    subroutine test_assertEqual_string()
        integer :: status
        call assertEqual("a", "a", status, assert_mock)
        call assertEqual(0, status, status, assert)

        call assertEqual("a", "b", status, assert_mock)
        call assertEqual(1, status, status, assert)
    end subroutine test_assertEqual_string

end module Test_fortran_test_framework_mod
