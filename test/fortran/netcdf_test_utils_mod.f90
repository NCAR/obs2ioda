module netcdf_test_utils_mod
    public :: file_exists, delete_file
contains
    ! Helper: Check if a file exists
    logical function file_exists(path)
        character(len = *), intent(in) :: path
        logical :: exists
        inquire(file = path, exist = exists)
        file_exists = exists
    end function file_exists

    ! Helper: Delete file if it exists
    subroutine delete_file(path)
        character(len = *), intent(in) :: path
        if (file_exists(path)) then
            open(unit = 99, file = path, status = 'old')
            close(99, status = 'delete')
        end if
    end subroutine delete_file
end module netcdf_test_utils_mod