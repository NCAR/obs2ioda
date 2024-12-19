module f_c_string_i_mod
    use iso_c_binding, only: c_ptr, c_size_t
    implicit none

    public :: strlen

    interface
        function strlen(c_string) bind(C, name="strlen") result(n)
            import :: c_ptr, c_size_t
            type(c_ptr), value :: c_string
            integer(c_size_t) :: n
        end function strlen
    end interface
end module f_c_string_i_mod