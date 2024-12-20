module f_c_string_t_mod
    use iso_c_binding, only : c_loc, c_ptr, c_null_char, c_char, c_f_pointer, c_null_ptr
    use f_c_string_i_mod, only : strlen
    implicit none

    type :: f_c_string_t
        ! Allocatable C-compatible null-terminated string
        character(len = :, kind = c_char), allocatable :: fc_string

    contains
        ! Type-bound procedures
        procedure :: to_c => to_c
        procedure :: to_f => to_f
        final :: cleanup
    end type f_c_string_t

contains
    ! Convert the Fortran string to a C-compatible null-terminated string.
    function to_c(this, f_string) result(c_string)
        class(f_c_string_t), target, intent(inout) :: this
        character(len = *), intent(in) :: f_string
        type(c_ptr) :: c_string
        integer :: n
        n = len(f_string)
        if (allocated(this%fc_string)) then
            deallocate(this%fc_string)
        end if
        allocate(character(len = n + 1) :: this%fc_string)
        this%fc_string = f_string // c_null_char
        c_string = c_loc(this%fc_string)
    end function to_c

    function to_f(this, c_string) result(f_string)
        class(f_c_string_t), intent(inout) :: this
        type(c_ptr), intent(in) :: c_string
        character(len = :), allocatable :: f_string
        character(len = 1, kind = c_char), pointer :: fc_string_pointer(:)
        integer :: n
        n = strlen(c_string)
        if (allocated(f_string)) then
            deallocate(f_string)
        end if
        allocate(character(len = n) :: f_string)
        call c_f_pointer(c_string, fc_string_pointer, [n + 1])
        f_string = transfer(fc_string_pointer(1:n), f_string)
    end function to_f

    ! Clean up the C-compatible null-terminated string.
    subroutine cleanup(this)
        type(f_c_string_t), intent(inout) :: this
        if (allocated(this%fc_string)) then
            deallocate(this%fc_string)
        end if
    end subroutine cleanup

end module f_c_string_t_mod

