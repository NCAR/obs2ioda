module netcdf_cxx_mod
    use iso_c_binding, only : c_int, c_ptr
    use f_c_string_t_mod, only : f_c_string_t
    use netcdf_cxx_i_mod, only : c_netcdfCreate, c_netcdfClose
    use netcdf, only : NF90_INT, NF90_REAL
    implicit none
    public

contains

    ! netcdfCreate:
    !   Creates a new NetCDF file or opens an existing file in a specified mode,
    !   using a Fortran string for the file path. This function wraps the
    !   `c_netcdfCreate` interface, which calls a C++ wrapper function that
    !   interacts with the NetCDF C++ API.
    !
    !   Arguments:
    !     - path (character(len=*), intent(in)): The file path as a Fortran string.
    !     - netcdfID (integer(c_int), intent(inout)): On input, it may contain an
    !       identifier to be updated; on output, it holds the file identifier
    !       for the created or opened NetCDF file.
    !     - fileMode (integer(c_int), intent(in), optional):
    !         File mode for creating or opening the NetCDF file. Defaults to 2
    !         (replace mode). Possible values are:
    !           - 0: Open an existing file in read-only mode.
    !           - 1: Open an existing file for writing.
    !           - 2: Create a new file, overwriting any existing file.
    !           - 3: Create a new file, failing if the file already exists.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating success (0) or failure (non-zero).
    !
    !   Notes:
    !     - The `f_c_string_t` type is used internally to handle the conversion
    !       of the Fortran string `path` into a null-terminated C string (`c_path`).
    !     - The `c_netcdfCreate` function serves as an interface between Fortran
    !       and the C++ wrapper function, ensuring proper communication between
    !       the languages.
    function netcdfCreate(path, netcdfID, fileMode)
        character(len = *), intent(in) :: path
        integer(c_int), intent(inout) :: netcdfID
        integer(c_int), intent(in), optional :: fileMode
        integer(c_int) :: netcdfCreate
        type(f_c_string_t) :: f_c_string_path
        type(c_ptr) :: c_path
        integer(c_int) :: mode
        ! Set the mode to the provided fileMode if present, otherwise default to 2
        if (present(fileMode)) then
            mode = fileMode
        else
            mode = 2
        end if
        c_path = f_c_string_path%to_c(path)
        netcdfCreate = c_netcdfCreate(c_path, netcdfID, mode)
    end function netcdfCreate

    ! netcdfClose:
    !   Closes a previously opened NetCDF file identified by its file identifier.
    !   This function wraps the `c_netcdfClose` interface, which calls a C++
    !   wrapper function to interact with the NetCDF C++ API.
    !
    !   Arguments:
    !     - netcdfID (integer(c_int), intent(in), value): The identifier of the
    !       NetCDF file to close.
    !
    !   Returns:
    !     - integer(c_int): A status code indicating success (0) or failure (non-zero).
    !
    !   Notes:
    !     - The `c_netcdfClose` function acts as an interface between Fortran and
    !       the C++ wrapper function, abstracting the complexity of C++ interactions.
    function netcdfClose(netcdfID)
        integer(c_int), value, intent(in) :: netcdfID
        integer(c_int) :: netcdfClose
        netcdfClose = c_netcdfClose(netcdfID)
    end function netcdfClose

end module netcdf_cxx_mod
