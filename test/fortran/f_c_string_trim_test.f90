subroutine f_c_string_t_to_c_trim_test()
    use test_utils_mod
    use f_c_string_t_mod
    use iso_c_binding
    implicit none
    integer :: status

    type(f_c_string_t) :: f_c_string
    type(c_ptr) :: c_string
    integer :: n
    c_string = f_c_string%to_c("abc   ")
    n = c_strlen(c_string)
    call assertEqual(3, n, status, assert)
end subroutine

subroutine f_c_string_t_to_f_trim_test()
    use test_utils_mod
    use f_c_string_t_mod
    use iso_c_binding
    implicit none
    integer :: status

    type(f_c_string_t) :: f_c_string
    type(c_ptr) :: c_string
    character(len=:), allocatable :: f_string
    c_string = f_c_string%to_c("abc   ")
    f_string = f_c_string%to_f(c_string)
    call assertequal("abc", f_string, status, assert)
    call assertequal(3, len(f_string), status, assert)
end subroutine

subroutine f_c_string_1D_t_to_f_trim_test()
    use test_utils_mod
    use f_c_string_1D_t_mod
    use iso_c_binding
    implicit none
    integer :: status, i

    type(f_c_string_1D_t) :: f_c_string_1D
    type(c_ptr) :: c_string_1D
    character(len=:), allocatable :: f_string_1D(:)

    c_string_1D = f_c_string_1D%to_c(["abc   ", "def   ", "ghij  "])
    f_string_1D = f_c_string_1D%to_f(c_string_1D, 3)
    do i = 1, size(f_string_1D)
        call assertequal(4, len(f_string_1D(i)), status, assert)
    end do
    call assertequal("abc", f_string_1D(1), status, assert)
    call assertequal("def", f_string_1D(2), status, assert)
    call assertequal("ghij", f_string_1D(3), status, assert)
end subroutine

program netcdf_file_test

    call f_c_string_t_to_c_trim_test()
    call f_c_string_t_to_f_trim_test()
    call f_c_string_1D_t_to_f_trim_test()

end program netcdf_file_test