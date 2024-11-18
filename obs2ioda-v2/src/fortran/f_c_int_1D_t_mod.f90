module f_c_int_1D_t_mod
    use iso_c_binding, only : c_ptr, c_int, c_loc, c_f_pointer
    use kinds, only : i_kind
    implicit none

    type, public :: f_c_int_1D_t
        ! Allocatable Fortran 1D integer array
        integer(i_kind), allocatable, dimension(:) :: f_int_1D
        ! Allocatable C-compatible integer array
        integer(kind=c_int), allocatable, dimension(:) :: fc_int_1D
        ! C pointer to the integer array
        type(c_ptr) :: c_int_1D
        ! Number of elements in the array
        integer :: n = -1
    contains
        procedure :: to_c
        procedure :: to_f
    end type f_c_int_1D_t

contains

    ! Convert the Fortran 1D integer array to a C-compatible integer array
    subroutine to_c(this)
        class(f_c_int_1D_t), target, intent(inout) :: this

        this%n = size(this%f_int_1D)
        ! Copy Fortran integers to C-compatible integers
        allocate(this%fc_int_1D(size(this%f_int_1D)))
        this%fc_int_1D = this%f_int_1D
        this%c_int_1D = c_loc(this%fc_int_1D)
    end subroutine to_c

    ! Convert a C-compatible integer array to a Fortran integer array
    subroutine to_f(this)
        class(f_c_int_1D_t), intent(inout) :: this
        integer(kind = c_int), pointer :: fc_int_1D_pointer(:)
        if (this%n < 0) then
            return
        end if
        ! Allocate and convert the C-compatible array back to Fortran integers
        allocate(this%f_int_1D(this%n))
        call c_f_pointer(this%c_int_1D, fc_int_1D_pointer, [this%n])
        this%f_int_1D = fc_int_1D_pointer
    end subroutine to_f

end module f_c_int_1D_t_mod
