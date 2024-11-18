module f_c_double_1D_t_mod
    use iso_c_binding, only : c_ptr, c_double, c_loc, c_f_pointer
    use kinds, only : r_double
    implicit none

    type, public :: f_c_double_1D_t
        ! Allocatable Fortran 1D double-precision array
        real(r_double), allocatable, dimension(:) :: f_double_1D
        ! Allocatable C-compatible double-precision array
        real(kind=c_double), allocatable, dimension(:) :: fc_double_1D
        ! C pointer to the double-precision array
        type(c_ptr) :: c_double_1D
        ! Number of elements in the array
        integer :: n
    contains
        procedure :: to_c
        procedure :: to_f
    end type f_c_double_1D_t

contains

    ! Convert the Fortran 1D double-precision array to a C-compatible array
    subroutine to_c(this)
        class(f_c_double_1D_t), target, intent(inout) :: this

        ! Copy Fortran doubles to C-compatible doubles
        allocate(this%fc_double_1D(size(this%f_double_1D)))
        this%fc_double_1D = this%f_double_1D
        this%c_double_1D = c_loc(this%fc_double_1D)
        this%n = size(this%f_double_1D)
    end subroutine to_c

    ! Convert a C-compatible double-precision array to a Fortran array
    subroutine to_f(this)
        class(f_c_double_1D_t), intent(inout) :: this
        real(kind = c_double), pointer :: fc_double_1D_pointer(:)

        ! Allocate and convert the C-compatible array back to Fortran doubles
        allocate(this%f_double_1D(this%n))
        call c_f_pointer(this%c_double_1D, fc_double_1D_pointer, [this%n])
        this%f_double_1D = fc_double_1D_pointer
    end subroutine to_f

end module f_c_double_1D_t_mod
