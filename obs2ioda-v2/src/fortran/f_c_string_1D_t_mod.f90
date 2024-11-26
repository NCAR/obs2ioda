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
        ! Type-bound procedures
        procedure :: init => init
        procedure :: to_c => to_c
        procedure :: to_f => to_f
        procedure :: cleanup => cleanup
    end type f_c_string_1D_t

contains
    ! Convert the Fortran string to a C-compatible null-terminated string.
    subroutine init(this, m, n)
        class(f_c_string_1D_t), target, intent(inout) :: this
        integer, intent(in) :: m, n
        character(len=n), dimension(m) :: a
        integer :: i
        this%m = m
        this%n = n
        allocate(this%f_c_string_t_array(this%m))
        allocate(this%fc_string_1D(this%m))
        a = repeat('a', n)
        this%f_string_1D = a
        do i = 1, this%m
            this%f_c_string_t_array(i)%f_string = this%f_string_1D(i)
            call this%f_c_string_t_array(i)%to_c()
            this%fc_string_1D(i) = this%f_c_string_t_array(i)%c_string
        end do
        this%c_string_1D = c_loc(this%fc_string_1D)
    end subroutine init
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

