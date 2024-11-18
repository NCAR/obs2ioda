module f_c_real_1D_t_mod
    use iso_c_binding, only : c_ptr, c_float, c_loc, c_f_pointer
    use kinds, only : r_kind
    implicit none

    type, public :: f_c_real_1D_t
        real(r_kind), allocatable, dimension(:) :: f_real_1D
        real(kind=c_float), allocatable, dimension(:) :: fc_real_1D
        type(c_ptr) :: c_real_1D
        integer :: n = -1
    contains
        procedure :: to_c
        procedure :: to_f
    end type f_c_real_1D_t
    contains
        subroutine to_c(this)
            class(f_c_real_1D_t), target, intent(inout) :: this
            integer :: i
            this%n = size(this%f_real_1D)
            this%fc_real_1D = this%f_real_1D
            this%c_real_1D = c_loc(this%fc_real_1D)
        end subroutine to_c
        subroutine to_f(this)
            class(f_c_real_1D_t), intent(inout) :: this
            real(kind = c_float), pointer :: fc_real_1D_pointer(:)
            if (this%n < 0) then
                return
            end if
            allocate(real(kind = c_float) :: this%fc_real_1D(this%n))
            call c_f_pointer(this%c_real_1D, fc_real_1D_pointer, [this%n])
            this%f_real_1D = fc_real_1D_pointer
        end subroutine to_f
end module f_c_real_1D_t_mod