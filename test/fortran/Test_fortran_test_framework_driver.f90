program Test_fortran_test_framework_driver
    use Test_fortran_test_framework_mod
    use fortran_test_framework_mod, only : assert, assert_memcheck, determine_test_type
    implicit none
    integer n_args
    character(len = 64) :: test_type, arg
    integer :: i
    procedure(assert_interface), pointer :: assert_proc

    call determine_test_type(test_type)

    if (trim(test_type) == "standard") then
        assert_proc => assert
    else
        assert_proc => assert_memcheck
    end if
    call test_assertEqual_integer(assert_proc)
    call test_assertEqual_logical(assert_proc)
    call test_assertEqual_string(assert_proc)
end program