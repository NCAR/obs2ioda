module fortran_test_framework_mod
    implicit none

    public :: assertEqual

    interface assertEqual
        module procedure assertEqual_integer
        module procedure assertEqual_string
        module procedure assertEqual_logical
    end interface assertEqual

    abstract interface
        subroutine assert_interface(condition, message, status)
            logical, intent(in) :: condition
            character(len = *), intent(in) :: message
            integer, intent(out) :: status
        end subroutine assert_interface
    end interface

contains

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
            call exit(1)
        else
            status = 0
            write(*, '(A)') "Success: " // message
        end if
    end subroutine assert

    ! Subroutine: assertEqual_integer
    ! Asserts that two integer values are equal, using a custom or default assertion handler.
    !
    ! Arguments:
    ! - expected (integer, in): The expected value.
    ! - actual (integer, in): The actual value.
    ! - status (integer, out): Status code (0 for success, 1 for failure).
    ! - assert_procedure (procedure): Procedure to handle assertion logic.
    subroutine assertEqual_integer(expected, actual, status, assert_procedure)
        implicit none
        integer, intent(in) :: expected, actual
        integer, intent(out) :: status
        procedure(assert_interface) :: assert_procedure
        procedure(assert_interface), pointer :: assert_handler => assert

        if (present(assert_procedure)) then
            assert_handler => assert_procedure
        end if

        call assert_handler(&
                expected == actual, "expected=" // trim(adjustl(itoa(expected))) // &
                        " actual=" // trim(adjustl(itoa(actual))), &
                status &
                )
    end subroutine assertEqual_integer

    ! Subroutine: assertEqual_string
    ! Asserts that two strings are equal, using a custom or default assertion handler.
    !
    ! Arguments:
    ! - expected (character, in): The expected string value.
    ! - actual (character, in): The actual string value.
    ! - status (integer, out): Status code (0 for success, 1 for failure).
    ! - assert_procedure (procedure): Procedure to handle assertion logic.
    subroutine assertEqual_string(expected, actual, status, assert_procedure)
        implicit none
        character(len = *), intent(in) :: expected, actual
        integer, intent(out) :: status
        procedure(assert_interface) :: assert_procedure
        procedure(assert_interface), pointer :: assert_handler => assert

        if (present(assert_procedure)) then
            assert_handler => assert_procedure
        end if

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
    ! - assert_procedure (procedure): Procedure to handle assertion logic.
    subroutine assertEqual_logical(expected, actual, status, assert_procedure)
        implicit none
        logical, intent(in) :: expected, actual
        integer, intent(out) :: status
        procedure(assert_interface) :: assert_procedure
        procedure(assert_interface), pointer :: assert_handler => assert

        if (present(assert_procedure)) then
            assert_handler => assert_procedure
        end if

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
        character(len = 32) :: str
        write(str, '(I0)') value
    end function itoa

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

end module fortran_test_framework_mod
