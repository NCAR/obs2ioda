module f_c_string_t_mod
    use iso_c_binding, only : c_loc, c_ptr, c_null_char, c_char, c_f_pointer, c_null_ptr
    use f_c_string_i_mod, only : strlen
    implicit none

    type :: f_c_string_t
        ! Allocatable Fortran string
        character(len = :), allocatable :: f_string
        ! Allocatable C-compatible null-terminated string
        character(len = :, kind = c_char), allocatable :: fc_string
        ! C pointer to the null-terminated string
        type(c_ptr) :: c_string
        ! Length of the Fortran string
        integer :: n = -1

    contains
        ! Type-bound procedures
        procedure :: to_c => to_c1
        procedure :: to_c1 => to_c1
        procedure :: to_c2 => to_c2
        procedure :: to_f => to_f1
        procedure :: to_f1 => to_f1
        procedure :: to_f2 => to_f2
        procedure :: cleanup => cleanup
    end type f_c_string_t

contains

    ! Convert the Fortran string to a C-compatible null-terminated string.
    subroutine to_c1(this)
        class(f_c_string_t), target, intent(inout) :: this
        this%n = len(this%f_string)
        allocate(character(len = this%n + 1) :: this%fc_string)
        this%fc_string = this%f_string // c_null_char
        this%c_string = c_loc(this%fc_string)
    end subroutine to_c1

    ! Convert the Fortran string to a C-compatible null-terminated string.
    function to_c2(this, f_string) result(c_string)
        class(f_c_string_t), target, intent(inout) :: this
        character(len = :), allocatable, intent(in) :: f_string
        type(c_ptr) :: c_string
        integer :: n
        n = len(f_string)
        allocate(character(len = n + 1) :: this%fc_string)
        this%fc_string = f_string // c_null_char
        c_string = c_loc(this%fc_string)
    end function to_c2

    ! Convert a C-compatible null-terminated string to a Fortran string.
    subroutine to_f1(this)
        class(f_c_string_t), intent(inout) :: this
        character(len = 1, kind = c_char), pointer :: fc_string_pointer(:)
        this%n = strlen(this%c_string)
        if (this%n < 0) then
            return
        end if
        if (allocated(this%f_string)) then
            deallocate(this%f_string)
        end if
        allocate(character(len = this%n) :: this%f_string)
        call c_f_pointer(this%c_string, fc_string_pointer, [this%n + 1])
        this%f_string = transfer(fc_string_pointer(1:this%n), this%f_string)
    end subroutine to_f1

    function to_f2(this, c_string) result(f_string)
        class(f_c_string_t), intent(inout) :: this
        type(c_ptr), intent(in) :: c_string
        character(len = :), allocatable :: f_string
        character(len = 1, kind = c_char), pointer :: fc_string_pointer(:)
        integer :: n
        n = strlen(c_string)
        if (n < 0) then
            return
        end if
        if (allocated(f_string)) then
            deallocate(f_string)
        end if
        allocate(character(len = n) :: f_string)
        call c_f_pointer(c_string, fc_string_pointer, [n + 1])
        f_string = transfer(fc_string_pointer(1:n), f_string)
    end function to_f2

    ! Cleanup method to deallocate allocated resources.
    subroutine cleanup(this)
        class(f_c_string_t), intent(inout) :: this

        if (allocated(this%fc_string)) then
            deallocate(this%fc_string)
        end if
        if (allocated(this%f_string)) then
            deallocate(this%f_string)
        end if

        this%c_string = c_null_ptr
        this%n = -1
    end subroutine cleanup

end module f_c_string_t_mod

