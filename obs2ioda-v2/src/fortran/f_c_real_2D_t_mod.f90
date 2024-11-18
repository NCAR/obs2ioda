module f_c_real_2D_t_mod
    use iso_c_binding, only : c_ptr, c_float, c_loc, c_f_pointer
    use f_c_real_1D_t_mod, only : f_c_real_1D_t
    use kinds, only : r_kind
    implicit none

    type, public :: f_c_real_2D_t
        real(r_kind), allocatable, dimension(:, :) :: f_real_2D
        real(kind=c_float), allocatable, dimension(:) :: fc_real_2D
        type(c_ptr) :: c_real_2D
        integer :: m, n
    contains
        procedure :: to_c
        procedure :: to_f
    end type f_c_real_2D_t
contains
    subroutine to_c(this)
        class(f_c_real_2D_t), target, intent(inout) :: this
        integer :: i
        this%m = size(this%f_real_2D, 1)
        this%n = size(this%f_real_2D, 2)
        this%fc_real_2D = reshape(this%f_real_2D, [this%m * this%n])
        this%c_real_2D = c_loc(this%fc_real_2D)
    end subroutine to_c
    subroutine to_f(this)
        class(f_c_real_2D_t), intent(inout) :: this
        real(kind = c_float), pointer :: fc_real_2D_pointer(:)
        allocate(real(kind = c_float) :: this%fc_real_2D(this%n))
        call c_f_pointer(this%c_real_2D, fc_real_2D_pointer, [this%m*this%n])
        this%f_real_2D = reshape(fc_real_2D_pointer, [this%m, this%n])
    end subroutine to_f
end module f_c_real_2D_t_mod