module write_ioda_v3_test_mod
    use define_mod, only: r_kind, i_kind
    implicit none

    type :: write_ioda_v3_test_t
        character(len=:), allocatable :: fname
        integer(i_kind) :: nlocs, nvars, nchans, nstring, ndatetime
        character(len=20), dimension(:), allocatable :: datetime
        character(len=10), dimension(:), allocatable :: name_var_tb
        real(r_kind), dimension(:), allocatable :: lat_out, lon_out
        real(r_kind), dimension(:), allocatable :: scan_pos_out
        real(r_kind), dimension(:), allocatable :: sat_zen_out, sat_azi_out
        real(r_kind), dimension(:), allocatable :: sun_zen_out, sun_azi_out
        real(r_kind), dimension(:,:), allocatable :: bt_out, err_out, qf_out

    contains
        procedure :: setup
    end type

contains

    subroutine setup(self)
        class(write_ioda_v3_test_t), intent(inout) :: self
        integer :: i, j
        character(len=100) :: output_dir

        ! Get output directory from command line, default to current directory
        call get_command_argument(1, output_dir)
        if (trim(output_dir) == '') then
            output_dir = './'
        else if (output_dir(len_trim(output_dir):len_trim(output_dir)) /= '/') then
            output_dir = trim(output_dir) // '/'
        end if

        ! Set configuration
        self%fname     = trim(output_dir)//"write_ioda_v3.nc"
        self%nlocs     = 2
        self%nvars     = 1
        self%nchans    = 3
        self%nstring   = 10
        self%ndatetime = 20

        ! Allocate arrays
        allocate(self%datetime(self%nlocs))
        allocate(self%name_var_tb(self%nvars))
        allocate(self%lat_out(self%nlocs), self%lon_out(self%nlocs), self%scan_pos_out(self%nlocs))
        allocate(self%sat_zen_out(self%nlocs), self%sat_azi_out(self%nlocs))
        allocate(self%sun_zen_out(self%nlocs), self%sun_azi_out(self%nlocs))
        allocate(self%bt_out(self%nchans, self%nlocs))
        allocate(self%err_out(self%nchans, self%nlocs))
        allocate(self%qf_out(self%nchans, self%nlocs))

        ! Metadata
        self%datetime       = ["20250101T0000Z", "20250101T0010Z"]
        self%name_var_tb(1) = "brightness_temperature"
        self%lat_out        = [45.0_r_kind, 46.0_r_kind]
        self%lon_out        = [-120.0_r_kind, -121.0_r_kind]
        self%scan_pos_out   = [0.0_r_kind, 1.0_r_kind]
        self%sat_zen_out    = [30.0_r_kind, 31.0_r_kind]
        self%sat_azi_out    = [150.0_r_kind, 151.0_r_kind]
        self%sun_zen_out    = [60.0_r_kind, 61.0_r_kind]
        self%sun_azi_out    = [100.0_r_kind, 101.0_r_kind]

        ! Brightness temperatures
        self%bt_out = reshape([270.0_r_kind, 271.0_r_kind, 272.0_r_kind, &
                273.0_r_kind, 274.0_r_kind, 275.0_r_kind], shape(self%bt_out))

        ! Error and quality flag values with unique patterns
        do j = 1, self%nlocs
            do i = 1, self%nchans
                self%err_out(i, j) = 10.0_r_kind * i + j
                self%qf_out(i, j)  = 100.0_r_kind * i + j
            end do
        end do

    end subroutine setup

end module write_ioda_v3_test_mod


program test_write_ioda_v3
    use goes_abi_converter_mod
    use define_mod, only: r_kind, i_kind, missing_r, missing_i
    use write_ioda_v3_test_mod
    implicit none

    type(write_ioda_v3_test_t) :: d

    ! Initialize test data
    call d%setup()

    ! Write IODA v3 NetCDF file
    call write_iodav3_netcdf(d%fname, d%nlocs, d%nvars, d%nchans, d%nstring, d%ndatetime, &
            missing_r, missing_i, d%datetime, d%lat_out, d%lon_out, &
            d%scan_pos_out, d%sat_zen_out, d%sat_azi_out, &
            d%sun_zen_out, d%sun_azi_out, d%bt_out, d%err_out, d%qf_out, d%name_var_tb)

end program test_write_ioda_v3
