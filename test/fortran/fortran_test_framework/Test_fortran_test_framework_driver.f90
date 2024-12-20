program Test_fortran_test_framework_driver
    use Test_fortran_test_framework_mod
    implicit none

    call test_assertEqual_string()
    call test_assertEqual_logical()
    call test_assertEqual_integer()
end program
