program test_write_netcdf_obs
    use write_netcdf_obs_mod
    implicit none
    ! Declarations
    character(len=256) :: fname = "test_minimal.nc"
    character(len=50) :: var_tb = "brightness_temperature"
    integer :: nvars = 1, nlocs = 1, nstring = 50, ndatetime = 1, nband = 1
    character(len=19), dimension(1) :: datetime = ["20250101T000000Z"]
    character(len=32), dimension(1) :: name_var_tb = ["brightness_temperature"]
    real, dimension(1,1) :: bt_out = reshape([300.0], shape(bt_out))
    real, dimension(1,1) :: err_out = reshape([1.0], shape(err_out))
    integer, dimension(1,1) :: qf_out = reshape([0], shape(qf_out))
    real, dimension(1) :: lat_out = [45.0]
    real, dimension(1) :: lon_out = [-105.0]
    real, dimension(1) :: sun_azi_out = [180.0]
    real, dimension(1) :: scan_pos_out = [0.0]
    real, dimension(1) :: sat_azi_out = [190.0]
    real, dimension(1) :: sun_zen_out = [45.0]
    real, dimension(1) :: sat_zen_out = [50.0]

    call write_obs_netcdf(fname, var_tb, nvars, nlocs, nstring, ndatetime, nband, &
            datetime, name_var_tb, bt_out, err_out, qf_out, &
            lat_out, lon_out, sun_azi_out, scan_pos_out, sat_azi_out, &
            sun_zen_out, sat_zen_out)

end program test_write_netcdf_obs