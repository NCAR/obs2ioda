module test_utils_test_mod
    use test_utils_mod, only : assertEqual, assert_interface
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
    ! Arguments:
    ! - assert_proc (procedure): The assertion procedure to use for testing.
    !
    ! Behavior:
    ! - Asserts equality of integers using both the mock assertion handler (`assert_mock`)
    !   and the default `assert` procedure.
    ! - Verifies correct handling of equal and unequal integer values.
    subroutine test_assertEqual_integer(assert_proc)
        procedure(assert_interface) :: assert_proc
        integer :: status
        call assertEqual(1, 1, status, assert_mock)
        call assertEqual(0, status, status, assert_proc)

        call assertEqual(1, 2, status, assert_mock)
        call assertEqual(1, status, status, assert_proc)
    end subroutine test_assertEqual_integer

    ! Subroutine: test_assertEqual_logical
    ! Tests the `assertEqual` subroutine with logical values.
    !
    ! Arguments:
    ! - assert_proc (procedure): The assertion procedure to use for testing.
    !
    ! Behavior:
    ! - Asserts equality of logical values (`.true.` and `.false.`) using both the mock
    !   assertion handler (`assert_mock`) and the default `assert` procedure.
    ! - Verifies correct handling of equal and unequal logical values.
    subroutine test_assertEqual_logical(assert_proc)
        procedure(assert_interface) :: assert_proc
        integer :: status
        call assertEqual(.true., .true., status, assert_mock)
        call assertEqual(0, status, status, assert_proc)

        call assertEqual(.true., .false., status, assert_mock)
        call assertEqual(1, status, status, assert_proc)
    end subroutine test_assertEqual_logical

    ! Subroutine: test_assertEqual_string
    ! Tests the `assertEqual` subroutine with string values.
    !
    ! Arguments:
    ! - assert_proc (procedure): The assertion procedure to use for testing.
    !
    ! Behavior:
    ! - Asserts equality of strings using both the mock assertion handler (`assert_mock`)
    !   and the default `assert` procedure.
    ! - Verifies correct handling of equal and unequal string values.
    subroutine test_assertEqual_string(assert_proc)
        procedure(assert_interface) :: assert_proc
        integer :: status
        call assertEqual("a", "a", status, assert_mock)
        call assertEqual(0, status, status, assert_proc)

        call assertEqual("a", "b", status, assert_mock)
        call assertEqual(1, status, status, assert_proc)
    end subroutine test_assertEqual_string

end module test_utils_test_mod

program test_utils_test
    use test_utils_test_mod, only : test_assertEqual_integer, test_assertEqual_logical, test_assertEqual_string
    use test_utils_mod, only : assert, assert_log, determine_test_type, assert_interface
    implicit none
    integer n_args
    character(len = 64) :: test_type, arg
    integer :: i
    procedure(assert_interface), pointer :: assert_proc

    call determine_test_type(test_type)

    if (trim(test_type) == "standard") then
        assert_proc => assert
    else
        assert_proc => assert_log
    end if
    call test_assertEqual_integer(assert_proc)
    call test_assertEqual_logical(assert_proc)
    call test_assertEqual_string(assert_proc)
end program