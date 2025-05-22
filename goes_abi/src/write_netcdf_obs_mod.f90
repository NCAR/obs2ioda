module write_netcdf_obs_mod
    public :: write_obs_netcdf
contains

    subroutine check(status)
        integer, intent(in) :: status
        integer, parameter :: success = 0
        if(status /= success) then
            print *, 'Error in write_obs_netcdf. Error code = ', status
            stop "Stopped"
        end if
    end subroutine check

    subroutine write_obs_netcdf(fname, var_tb, nvars, nlocs, nstring, ndatetime, nband, &
            datetime, name_var_tb, bt_out, err_out, qf_out, &
            lat_out, lon_out, sun_azi_out, scan_pos_out, sat_azi_out, &
            sun_zen_out, sat_zen_out)
        use netcdf_cxx_mod
        use kinds
        use netcdf
        use netcdf_mod
        implicit none

        ! Input arguments
        character(len = *), intent(in) :: fname, var_tb
        integer, intent(in) :: nvars, nlocs, nstring, ndatetime, nband
        character(len = *), dimension(:), intent(in) :: datetime
        character(len = *), dimension(:), intent(inout) :: name_var_tb
        real, dimension(:, :), intent(in) :: bt_out, err_out
        integer, dimension(:, :), intent(in) :: qf_out
        real, dimension(:), intent(in) :: lat_out, lon_out, sun_azi_out
        real, dimension(:), intent(in) :: scan_pos_out, sat_azi_out
        real, dimension(:), intent(in) :: sun_zen_out, sat_zen_out

        ! Local variables
        integer :: ncfileid, ncid_nvars, ncid_nlocs, ncid_nstring, ncid_ndatetime
        integer :: i
        character(len = 256) :: ncname
        character(len = 4) :: c4
        integer, parameter :: sensor_channels(10) = (/7, 8, 9, 10, 11, 12, 13, 14, 15, 16/)


        integer :: status, netcdfID, fileMode, ncid
        character(len=:), allocatable :: filename

        character(len=50) :: nvarsDimName, nlocsDimName, nstringDimName, ndatetimeDimName, channelDimName
        integer :: nvarsDimID, nlocsDimID, nstringDimID, ndatetimeDimID, channelDimID
        integer :: nchannels




        call check(netcdfCreate(fname, ncid))
        if (status /= 0) stop 'Failed to create file'

        ! Define groups
        call check(netcdfAddGroup(ncid, 'ObsValue'))
        call check(netcdfAddGroup(ncid, 'ObsError'))
        call check(netcdfAddGroup(ncid, 'PreQC'))
        call check(netcdfAddGroup(ncid, 'MetaData'))
        call check(netcdfAddGroup(ncid, 'VarMetaData'))

        ! Define dimensions (global)
        call check(netcdfAddDim(ncid, 'nvars', nvars, nvarsDimID))
        call check(netcdfAddDim(ncid, 'nlocs', nlocs, nlocsDimID))
        call check(netcdfAddDim(ncid, 'nstring', nstring, nstringDimID))
        call check(netcdfAddDim(ncid, 'ndatetime', ndatetime, ndatetimeDimID))

        ! Define per-band variables in groups
        do i = 1, nvars
            write(c4, '(i4)') i + 6
            name_var_tb(i) = trim(var_tb)//'_'//trim(adjustl(c4))

            call check(netcdfAddVar(ncid, name_var_tb(i), NF90_REAL, 1, ['nlocs'], 'ObsValue', fillValue=missing_r))
            call check(netcdfPutAtt(ncid, 'units', 'K', name_var_tb(i), 'ObsValue'))

            call check(netcdfAddVar(ncid, name_var_tb(i), NF90_REAL, 1, ['nlocs'], 'ObsError', fillValue=missing_r))
            call check(netcdfAddVar(ncid, name_var_tb(i), NF90_INT, 1, ['nlocs'], 'PreQC', fillValue=missing_i))
        end do
!
        ! Define metadata variables
        call check(netcdfAddVar(ncid, 'latitude', NF90_REAL, 1, ['nlocs'], 'MetaData', fillValue=missing_r))
        call check(netcdfAddVar(ncid, 'longitude', NF90_REAL, 1, ['nlocs'], 'MetaData', fillValue=missing_r))
        call check(netcdfAddVar(ncid, 'solar_azimuth_angle', NF90_REAL, 1, ['nlocs'], 'MetaData', fillValue=missing_r))
        call check(netcdfAddVar(ncid, 'scan_position', NF90_REAL, 1, ['nlocs'], 'MetaData', fillValue=missing_r))
        call check(netcdfAddVar(ncid, 'sensor_azimuth_angle', NF90_REAL, 1, ['nlocs'], 'MetaData', fillValue=missing_r))
        call check(netcdfAddVar(ncid, 'solar_zenith_angle', NF90_REAL, 1, ['nlocs'], 'MetaData', fillValue=missing_r))
        call check(netcdfAddVar(ncid, 'sensor_zenith_angle', NF90_REAL, 1, ['nlocs'], 'MetaData', fillValue=missing_r))
        call check(netcdfAddVar(ncid, 'sensor_view_angle', NF90_REAL, 1, ['nlocs'], 'MetaData', fillValue=missing_r))
        call check(netcdfAddVar(ncid, 'datetime', NF90_STRING, 1, ['nlocs'], 'MetaData', fillValue=" "))

        call check(netcdfAddVar(ncid, 'sensor_channel', NF90_INT, 1, ['nvars'], 'VarMetaData', fillValue=missing_i))
        call check(netcdfAddVar(ncid, 'variable_names', NF90_STRING, 1, ['nvars'], 'VarMetaData', fillValue=" "))
!
!        ! Write variables (example only)
!        ! Replace with real data as needed
        do i = 1, nvars
            call check(netcdfPutVar(ncid, name_var_tb(i), bt_out(i, :), 'ObsValue'))
            call check(netcdfPutVar(ncid, name_var_tb(i), err_out(i, :), 'ObsError'))
            call check(netcdfPutVar(ncid, name_var_tb(i), qf_out(i, :), 'PreQC'))
        end do
!
        call check(netcdfPutVar(ncid, 'latitude', lat_out, 'MetaData'))
        call check(netcdfPutVar(ncid, 'longitude', lon_out, 'MetaData'))
        call check(netcdfPutVar(ncid, 'solar_azimuth_angle', sun_azi_out, 'MetaData'))
        call check(netcdfPutVar(ncid, 'scan_position', scan_pos_out, 'MetaData'))
        call check(netcdfPutVar(ncid, 'sensor_azimuth_angle', sat_azi_out, 'MetaData'))
        call check(netcdfPutVar(ncid, 'solar_zenith_angle', sun_zen_out, 'MetaData'))
        call check(netcdfPutVar(ncid, 'sensor_zenith_angle', sat_zen_out, 'MetaData'))
        call check(netcdfPutVar(ncid, 'sensor_view_angle', sat_zen_out, 'MetaData'))
        call check(netcdfPutVar(ncid, 'sensor_channel', sensor_channels, 'VarMetaData'))
        call check(netcdfPutVar(ncid, 'datetime', datetime, 'MetaData'))
        call check(netcdfPutVar(ncid, 'variable_names', name_var_tb, 'VarMetaData'))

        call check(netcdfClose(ncid))
        if (status /= 0) stop 'Failed to close file'


    end subroutine write_obs_netcdf
end module write_netcdf_obs_mod