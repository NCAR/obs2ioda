! Module: test_utils_mod
! Provides utilities for test assertions and command-line test configuration.
!
! This module includes subroutines and functions for:
! - Performing assertions with custom handlers.
! - Comparing values of various types (integer, real, string, logical).
! - Determining the test type based on command-line arguments.
! - Converting data types to strings for reporting purposes.
!
! Key Features:
! - `assertEqual` interface for type-specific equality checks.
! - `determine_test_type` for setting test configurations.
! - Helper functions like `itoa` and `logical_to_string`.
module test_utils_mod
    implicit none

    private :: itoa, logical_to_string

    interface assertEqual
        module procedure assertEqual_integer
        module procedure assertEqual_float
        module procedure assertEqual_string
        module procedure assertEqual_logical
    end interface assertEqual

    ! Abstract Interface: assert_interface
    ! Defines a generic interface for assertion handlers.
    !
    ! This interface provides a standard structure for custom assertion subroutines,
    ! allowing flexibility in how assertion results are handled (e.g., printing messages,
    ! logging, or terminating the program).
    !
    ! Arguments:
    ! - condition (logical, in): The condition to evaluate.
    !   - Should be `.true.` for the assertion to pass.
    ! - message (character, in): The message associated with the assertion.
    !   - Typically used to describe the expected and actual values or the context of the check.
    ! - status (integer, out): Status code indicating the result of the assertion.
    !   - 0: Assertion passed.
    !   - 1: Assertion failed.
    !
    ! Example Implementation:
    !   subroutine custom_assert(condition, message, status)
    !       if (.not. condition) then
    !           status = 1
    !           write(*, *) "Assertion failed: ", message
    !       else
    !           status = 0
    !       end if
    !   end subroutine custom_assert
    abstract interface
        subroutine assert_interface(condition, message, status)
            logical, intent(in) :: condition
            character(len = *), intent(in) :: message
            integer, intent(out) :: status
        end subroutine assert_interface
    end interface

contains

    ! Subroutine: determine_test_type
    ! Determines the type of test to perform based on command-line arguments.
    !
    ! This subroutine reads the command-line arguments and sets the `test_type`
    ! output variable based on their contents. If no specific argument is provided,
    ! the test type defaults to "standard". If the "memcheck" argument is found,
    ! the test type is set to "memcheck".
    !
    ! Arguments:
    ! - test_type (character, out): The type of test to perform.
    !   Possible values:
    !     - "standard": Default test type.
    !     - "memcheck": Memory checking test.
    !
    ! Example Usage:
    !   call determine_test_type(test_type)
    !   if (test_type == "memcheck") then
    !       print *, "Performing memory check tests."
    !   else
    !       print *, "Performing standard tests."
    !   end if
    !
    ! Notes:
    ! - This subroutine uses `command_argument_count` and `get_command_argument`
    !   to retrieve and evaluate the command-line arguments.
    subroutine determine_test_type(test_type)
        implicit none
        character(len = *), intent(out) :: test_type
        integer n_args
        character(len = 64) :: arg
        integer :: i

        test_type = "standard"
        n_args = command_argument_count()
        do i = 1, n_args
            call get_command_argument(i, arg)
            if (trim(arg) == "memcheck") then
                test_type = "memcheck"
            end if
        end do
    end subroutine determine_test_type

    ! Subroutine: assert
    ! Performs a basic assertion by evaluating a condition and printing a message.
    ! Exits the program with an error status if the condition is not met.
    !
    ! Arguments:
    ! - condition (logical, in): The condition to evaluate.
    ! - message (character, in): Message to print indicating the assertion result.
    ! - status (integer, out): Status code (0 for success, 1 for failure).
    subroutine assert(condition, message, status)
        implicit none
        logical, intent(in) :: condition
        character(len = *), intent(in) :: message
        integer, intent(out) :: status
        if (.not. condition) then
            status = 1
            write(*, '(A)') "Failed: " // message
            stop 1
        else
            status = 0
            write(*, '(A)') "Success: " // message
        end if
    end subroutine assert

    ! Subroutine: assert_log
    ! An assertion handler that does not terminate
    ! the program on failure.
    !
    ! Arguments:
    ! - condition (logical, in): The condition to evaluate.
    ! - message (character, in): The message associated with the assertion.
    ! - status (integer, out): The status code set based on the condition (0 for success, 1 for failure).
    subroutine assert_log(condition, message, status)
        implicit none
        logical, intent(in) :: condition
        character(len = *), intent(in) :: message
        integer, intent(out) :: status

        if (.not. condition) then
            status = 1
            write(*, '(A)') "Failed: " // message
        else
            status = 0
            write(*, '(A)') "Success: " // message
        end if
    end subroutine assert_log

    ! Subroutine: assertEqual_integer
    ! Asserts that two integer values are equal, using a custom or default assertion handler.
    !
    ! Arguments:
    ! - expected (integer, in): The expected value.
    ! - actual (integer, in): The actual value.
    ! - status (integer, out): Status code (0 for success, 1 for failure).
    ! - assert_procedure (procedure): Custom procedure to handle assertion logic.
    subroutine assertEqual_integer(expected, actual, status, assert_procedure)
        implicit none
        integer, intent(in) :: expected, actual
        integer, intent(out) :: status
        procedure(assert_interface) :: assert_procedure
        procedure(assert_interface), pointer :: assert_handler => assert

        assert_handler => assert_procedure

        call assert_handler(&
                expected == actual, "expected=" // trim(adjustl(itoa(expected))) // &
                        " actual=" // trim(adjustl(itoa(actual))), &
                status &
                )
    end subroutine assertEqual_integer

    ! Subroutine: assertEqual_float
    ! Asserts that two float values are equal, using a custom or default assertion handler.
    !
    ! Arguments:
    ! - expected (real, in): The expected value.
    ! - actual (real, in): The actual value.
    ! - status (integer, out): Status code (0 for success, 1 for failure).
    ! - assert_procedure (procedure): Custom procedure to handle assertion logic.
    subroutine assertEqual_float(expected, actual, status, assert_procedure)
        implicit none
        real, intent(in) :: expected, actual
        integer, intent(out) :: status
        procedure(assert_interface) :: assert_procedure
        procedure(assert_interface), pointer :: assert_handler => assert

        assert_handler => assert_procedure

        call assert_handler(&
                expected == actual, "expected=" // trim(adjustl(ftoa(expected))) // &
                        " actual=" // trim(adjustl(ftoa(actual))), &
                status &
                )
    end subroutine assertEqual_float

    ! Subroutine: assertEqual_string
    ! Asserts that two strings are equal, using a custom or default assertion handler.
    !
    ! Arguments:
    ! - expected (character, in): The expected string value.
    ! - actual (character, in): The actual string value.
    ! - status (integer, out): Status code (0 for success, 1 for failure).
    ! - assert_procedure (procedure): Custom procedure to handle assertion logic.
    subroutine assertEqual_string(expected, actual, status, assert_procedure)
        implicit none
        character(len = *), intent(in) :: expected, actual
        integer, intent(out) :: status
        procedure(assert_interface) :: assert_procedure
        procedure(assert_interface), pointer :: assert_handler => assert

        assert_handler => assert_procedure

        call assert_handler(&
                expected == actual, "expected='" // trim(expected) // "' actual='" // &
                        trim(actual) // "'", &
                status &
                )
    end subroutine assertEqual_string

    ! Subroutine: assertEqual_logical
    ! Asserts that two logical values are equivalent, using a custom or default assertion handler.
    !
    ! Arguments:
    ! - expected (logical, in): The expected logical value.
    ! - actual (logical, in): The actual logical value.
    ! - status (integer, out): Status code (0 for success, 1 for failure).
    ! - assert_procedure (procedure): Custom procedure to handle assertion logic.
    subroutine assertEqual_logical(expected, actual, status, assert_procedure)
        implicit none
        logical, intent(in) :: expected, actual
        integer, intent(out) :: status
        procedure(assert_interface) :: assert_procedure
        procedure(assert_interface), pointer :: assert_handler => assert

        assert_handler => assert_procedure

        call assert_handler(&
                expected .eqv. actual, "expected=" // trim(logical_to_string(expected)) // &
                        " actual=" // trim(logical_to_string(actual)), &
                status &
                )
    end subroutine assertEqual_logical

    ! Function: itoa
    ! Converts an integer to a string representation.
    !
    ! Arguments:
    ! - value (integer, in): The integer to convert.
    !
    ! Returns:
    ! - str (character): String representation of the integer.
    function itoa(value) result(str)
        implicit none
        integer, intent(in) :: value
        character(len=32) :: str
        character(len=32) :: tmp
        integer :: ios

        write(tmp, '(I0)', iostat=ios) value
        if (ios /= 0) then
            str = '**overflow**'
        else
            str = tmp
        endif
    end function itoa

    ! Function: ftoa
    ! Converts a float to a string representation.
    !
    ! Arguments:
    ! - value (float, in): The float to convert.
    !
    ! Returns:
    ! - str (character): String representation of the float.
    function ftoa(value) result(str)
        implicit none
        real, intent(in) :: value
        character(len=32) :: str
        character(len=32) :: tmp
        integer :: ios

        write(tmp, '(F0.0)', iostat=ios) value
        if (ios /= 0) then
            str = '**overflow**'
        else
            str = tmp
        endif
    end function ftoa

    ! Function: logical_to_string
    ! Converts a logical value to a string representation (".true." or ".false.").
    !
    ! Arguments:
    ! - value (logical, in): The logical value to convert.
    !
    ! Returns:
    ! - str (character): String representation of the logical value.
    function logical_to_string(value) result(str)
        implicit none
        logical, intent(in) :: value
        character(len = 6) :: str

        if (value) then
            str = ".true."
        else
            str = ".false."
        end if
    end function logical_to_string

end module test_utils_mod
