module fortran_test_framework_mod
    implicit none

    interface assertEqual
        module procedure assertEqual_integer
        module procedure assertEqual_string
        module procedure assertEqual_logical
    end interface assertEqual

contains

    ! Generic assertion subroutine to handle error messages
    subroutine assert(condition, message)
        implicit none
        logical, intent(in) :: condition
        character(len=*), intent(in) :: message

        if (.not. condition) then
            print *, "Error: ", trim(message)
            call exit(1)
        else
            print *, "Success: ", trim(message)
        end if
    end subroutine assert

    ! Integer equality assertion
    subroutine assertEqual_integer(expected, actual)
        implicit none
        integer, intent(in) :: expected, actual

        call assert(expected == actual, "expected=" // trim(adjustl(itoa(expected))) // &
                " actual=" // trim(adjustl(itoa(actual))))
    end subroutine assertEqual_integer

    ! String equality assertion
    subroutine assertEqual_string(expected, actual)
        implicit none
        character(len=*), intent(in) :: expected, actual

        call assert(expected == actual, "expected='" // trim(expected) // "' actual='" // &
                trim(actual) // "'")
    end subroutine assertEqual_string

    ! Logical equality assertion
    subroutine assertEqual_logical(expected, actual)
        implicit none
        logical, intent(in) :: expected, actual

        call assert(expected .eqv. actual, "expected=" // trim(logical_to_string(expected)) // &
                " actual=" // trim(logical_to_string(actual)))
    end subroutine assertEqual_logical

    ! Assert true
    subroutine assertTrue(actual)
        implicit none
        logical, intent(in) :: actual

        call assert(actual, "expected=.true. actual=.false.")
    end subroutine assertTrue

    ! Assert false
    subroutine assertFalse(actual)
        implicit none
        logical, intent(in) :: actual

        call assert(.not. actual, "expected=.false. actual=.true.")
    end subroutine assertFalse

    ! Helper function to convert integer to string
    function itoa(value) result(str)
        implicit none
        integer, intent(in) :: value
        character(len=32) :: str
        write(str, '(I0)') value
    end function itoa

    ! Helper function to convert logical to string
    function logical_to_string(value) result(str)
        implicit none
        logical, intent(in) :: value
        character(len=6) :: str

        if (value) then
            str = ".true."
        else
            str = ".false."
        end if
    end function logical_to_string

end module fortran_test_framework_mod
