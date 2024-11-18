module f_c_string_t_mod
    use iso_c_binding, only : c_loc, c_ptr, c_null_char, c_char, c_f_pointer
    implicit none

    type :: f_c_string_t
        ! Allocatable Fortran string
        character(len = :), allocatable  :: f_string
        ! Allocatable C-compatible null-terminated string
        character(len = :, kind = c_char), allocatable :: fc_string
        ! C pointer to the null-terminated string
        type(c_ptr) :: c_string
        ! Length of the Fortran string
        integer :: n

        contains
        ! Type-bound procedures
        procedure :: to_c => to_c
        procedure :: to_f => to_f
    end type f_c_string_t

contains

    ! Convert the Fortran string to a C-compatible null-terminated string.
    subroutine to_c(this)
        class(f_c_string_t), target, intent(inout) :: this
        this%n = len(this%f_string)
        allocate(character(len = this%n + 1) :: this%fc_string)
        this%fc_string = this%f_string // c_null_char
        this%c_string = c_loc(this%fc_string)
    end subroutine to_c

    ! Convert a C-compatible null-terminated string to a Fortran string.
    subroutine to_f(this)
        class(f_c_string_t), intent(inout) :: this
        character(len=1, kind=c_char), pointer :: fc_string_pointer(:)

        allocate(character(len=this%n) :: this%f_string)
        call c_f_pointer(this%c_string, fc_string_pointer, [this%n + 1])
        this%f_string = transfer(fc_string_pointer(1:this%n), this%f_string)
    end subroutine to_f

end module f_c_string_t_mod

