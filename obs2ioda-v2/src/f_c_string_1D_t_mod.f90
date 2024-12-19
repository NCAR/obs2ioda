module f_c_string_1D_t_mod
    use iso_c_binding, only : c_loc, c_ptr, c_null_char, c_char, c_f_pointer, c_null_ptr
    use f_c_string_t_mod, only : f_c_string_t
    implicit none

    type :: f_c_string_1D_t
        ! Allocatable Fortran string
        character(len = :), allocatable :: f_string_1D(:)
        ! Allocatable C-compatible null-terminated string
        type(f_c_string_t), allocatable :: f_c_string_t_array(:)
        type(c_ptr), allocatable :: fc_string_1D(:)
        ! C pointer to the null-terminated string
        type(c_ptr) :: c_string_1D = c_null_ptr
        ! Length of the Fortran string
        integer :: m = -1, n = -1

    contains
        procedure :: to_c => to_c
        procedure :: to_c1 => to_c
        procedure :: to_c2 => to_c2
        procedure :: to_f => to_f
        procedure :: to_f1 => to_f
        procedure :: to_f2 => to_f2
        procedure :: cleanup => cleanup
    end type f_c_string_1D_t

contains
    ! Convert the Fortran string to a C-compatible null-terminated string.
    subroutine to_c(this)
        class(f_c_string_1D_t), target, intent(inout) :: this
        integer :: i
        this%m = size(this%f_string_1D)
        this%n = len(this%f_string_1D(1))
        allocate(this%f_c_string_t_array(this%m))
        allocate(this%fc_string_1D(this%m))
        do i = 1, this%m
            this%f_c_string_t_array(i)%f_string = this%f_string_1D(i)
            call this%f_c_string_t_array(i)%to_c()
            this%fc_string_1D(i) = this%f_c_string_t_array(i)%c_string
        end do
        this%c_string_1D = c_loc(this%fc_string_1D)

    end subroutine to_c

    function to_c2(this, f_string_1D) result(c_string_1D)
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
            this%fc_string_1D(i) = this%f_c_string_t_array(i)%to_c2(f_string)
        end do
        c_string_1D = c_loc(this%fc_string_1D)
    end function to_c2

    ! Convert a C-compatible null-terminated string to a Fortran string.
    subroutine to_f(this)
        class(f_c_string_1D_t), intent(inout) :: this
        type(c_ptr), pointer :: fc_string_1D_pointer(:)
        integer :: i
        if (this%m < 0) then
            return
        end if
        if (this%n < 0) then
            return
        end if
        if (allocated(this%f_string_1D)) then
            deallocate(this%f_string_1D)
        end if
        if (allocated(this%f_c_string_t_array)) then
            do i = 1, this%m
                call this%f_c_string_t_array(i)%cleanup()
            end do
            deallocate(this%f_c_string_t_array)
        end if
        if (allocated(this%fc_string_1D)) then
            deallocate(this%fc_string_1D)
        end if
        allocate(character(len = this%n) :: this%f_string_1D(1:this%m))
        allocate(this%f_c_string_t_array(this%m))
        allocate(this%fc_string_1D(this%m))
        call c_f_pointer(this%c_string_1D, fc_string_1D_pointer, [this%m])
        do i = 1, this%m
            this%f_c_string_t_array(i)%c_string = fc_string_1D_pointer(i)
            this%f_c_string_t_array(i)%n = this%n
            call this%f_c_string_t_array(i)%to_f()
            this%f_string_1D(i) = this%f_c_string_t_array(i)%f_string
        end do

    end subroutine to_f

    function to_f2(this, c_string_1D, m, n) result(f_string_1D)
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
        if (allocated(this%f_c_string_t_array)) then
            do i = 1, m
                call this%f_c_string_t_array(i)%cleanup()
            end do
            deallocate(this%f_c_string_t_array)
        end if
        if (allocated(this%fc_string_1D)) then
            deallocate(this%fc_string_1D)
        end if
        allocate(character(len = n) :: f_string_1D(1:m))
        allocate(this%f_c_string_t_array(m))
        allocate(this%fc_string_1D(m))
        call c_f_pointer(c_string_1D, fc_string_1D_pointer, [m])
        do i = 1, m
!            this%f_c_string_t_array(i)%c_string = fc_string_1D_pointer(i)
!            this%f_c_string_t_array(i)%n = n
            f_string_1D(i) = this%f_c_string_t_array(i)%to_f2(fc_string_1D_pointer(i))
!            f_string_1D(i) = this%f_c_string_t_array(i)%f_string
        end do

    end function to_f2

    subroutine cleanup(this)
        class(f_c_string_1D_t), intent(inout) :: this
        integer :: i
        if (allocated(this%fc_string_1D)) then
            deallocate(this%fc_string_1D)
        end if
        if (allocated(this%f_c_string_t_array)) then
            do i = 1, this%m
                call this%f_c_string_t_array(i)%cleanup()
            end do
            deallocate(this%f_c_string_t_array)
        end if
        if (allocated(this%f_string_1D)) then
            deallocate(this%f_string_1D)
        end if
        this%c_string_1D = c_null_ptr
        this%m = -1
        this%n = -1
    end subroutine cleanup

end module f_c_string_1D_t_mod
