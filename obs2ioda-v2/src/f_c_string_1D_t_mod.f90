module f_c_string_1D_t_mod
    use iso_c_binding, only : c_loc, c_ptr, c_null_char, c_char, c_f_pointer, c_null_ptr
    use f_c_string_t_mod, only : f_c_string_t
    implicit none

    type :: f_c_string_1D_t
        ! Allocatable C-compatible null-terminated string
        type(f_c_string_t), allocatable :: f_c_string_t_array(:)
        type(c_ptr), allocatable :: fc_string_1D(:)

    contains
        procedure :: to_c => to_c
        procedure :: to_f => to_f
    end type f_c_string_1D_t

contains

    function to_c(this, f_string_1D) result(c_string_1D)
        class(f_c_string_1D_t), target, intent(inout) :: this
        character(len = :), allocatable, intent(in) :: f_string_1D(:)
        character(len = :), allocatable :: f_string
        type(c_ptr) :: c_string_1D
        integer :: i, m, n
        m = size(f_string_1D)
        n = len(f_string_1D(1))
        allocate(this%f_c_string_t_array(m))
        allocate(this%fc_string_1D(m))
        do i = 1, m
            f_string = f_string_1D(i)
            this%fc_string_1D(i) = this%f_c_string_t_array(i)%to_c(f_string)
        end do
        c_string_1D = c_loc(this%fc_string_1D)
    end function to_c

    function to_f(this, c_string_1D, m, n) result(f_string_1D)
        class(f_c_string_1D_t), intent(inout) :: this
        type(c_ptr), intent(in) :: c_string_1D
        integer, intent(in) :: m, n
        character(len = :), allocatable :: f_string_1D(:)
        type(c_ptr), pointer :: fc_string_1D_pointer(:)
        integer :: i
        if (m < 0) then
            return
        end if
        if (n < 0) then
            return
        end if
        allocate(character(len = n) :: f_string_1D(1:m))
        allocate(this%f_c_string_t_array(m))
        allocate(this%fc_string_1D(m))
        call c_f_pointer(c_string_1D, fc_string_1D_pointer, [m])
        do i = 1, m
            f_string_1D(i) = this%f_c_string_t_array(i)%to_f(fc_string_1D_pointer(i))
        end do

    end function to_f

end module f_c_string_1D_t_mod
