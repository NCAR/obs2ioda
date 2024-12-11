module fortran_test_framework_mod
    implicit none
contains
    subroutine assertEqual(expected, actual)
        implicit none
        integer, intent(in) :: expected, actual

        if (expected /= actual) then
            print *, "Error: expected=", expected, " actual=", actual
            call exit(1)
        else
            print *, "Success: expected=", expected, " actual=", actual
        end if
    end subroutine assertEqual
end module fortran_test_framework_mod