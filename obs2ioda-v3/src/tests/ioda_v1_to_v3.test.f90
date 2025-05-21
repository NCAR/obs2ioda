program ioda_v1_to_v3_test
    use netcdf_cxx_mod
    use netcdf
    implicit none
    integer :: netcdfID, status, dimID
    character(len = :), allocatable :: testFileName, varName1, varName2, varName3
    integer :: ncid, varid, retval
    integer :: n1, n2, j, i, groupid
    integer :: dimids(2)
    real, allocatable :: data(:, :)
    real, allocatable :: brightness_temperature_1_data(:), brightness_temperature_2_data(:), brightness_temperature_3_data(:)
    real, allocatable :: brightness_temperature_data(:, :)

    allocate(brightness_temperature_1_data(10))
    allocate(brightness_temperature_2_data(10))
    allocate(brightness_temperature_3_data(10))
    allocate(brightness_temperature_data(3, 10))
    do i = 1, 10
        brightness_temperature_1_data(i) = i * 1.0
        brightness_temperature_2_data(i) = i * 2.0
        brightness_temperature_3_data(i) = i * 3.0
    end do
    brightness_temperature_data(1, :) = brightness_temperature_1_data
    brightness_temperature_data(2, :) = brightness_temperature_2_data
    brightness_temperature_data(3, :) = brightness_temperature_3_data

    testFileName = "test_ioda_v1_to_v3.nc"
    varName1 = "brightness_temperature_1@ObsValue"
    varName2 = "brightness_temperature_2@ObsValue"
    varName3 = "brightness_temperature_3@ObsValue"
    status = netcdfCreate(testFileName, netcdfID)
    if (status /= 0) then
        print *, "Error creating file: ", testFileName
        call exit(1)
    end if
    status = netcdfAddDim(netcdfID, "nlocs", 10, dimID)
    status = netcdfAddDim(netcdfID, "nchans", 3, dimID)
    status = netcdfAddVar(netcdfID, varName1, NF90_FLOAT, 1, ("nlocs"))
    status = netcdfAddVar(netcdfID, varName2, NF90_FLOAT, 1, ("nlocs"))
    status = netcdfAddVar(netcdfID, varName3, NF90_FLOAT, 1, ("nlocs"))
    if (status /= 0) then
        print *, "Error adding variable: ", varName1
        call exit(1)
    end if
    status = netcdfPutVar(netcdfID, varName1, brightness_temperature_1_data)
    if (status /= 0) then
        print *, "Error putting variable data: ", varName2
        call exit(1)
    end if
    status = netcdfPutVar(netcdfID, varName2, brightness_temperature_2_data)
    if (status /= 0) then
        print *, "Error putting variable data: ", varName3
        call exit(1)
    end if
    status = netcdfPutVar(netcdfID, varName3, brightness_temperature_3_data)
    if (status /= 0) then
        print *, "Error putting variable data: ", varName3
        call exit(1)
    end if
    status = netcdfClose(netcdfID)
    if (status /= 0) then
        print *, "Error closing file: ", testFileName
        call exit(1)
    end if

    status = nf90_open("test_ioda_v1_to_v3.nc", NF90_NOWRITE, ncid)
    retval = nf90_inq_ncid(ncid, "ObsValue", groupid)

    status = nf90_inq_varid(groupid, "brightnessTemperature", varid)
    status = nf90_inquire_variable(groupid, varid, dimids = dimids)
    status = nf90_inquire_dimension(groupid, dimids(1), len = n1)
    status = nf90_inquire_dimension(groupid, dimids(2), len = n2)

    allocate(data(n1, n2))
    status = nf90_get_var(groupid, varid, data)
    do i = 1, n1
        do j = 1, n2
            if (data(i, j) /= brightness_temperature_data(i, j)) then
                print *, "Error: data(", i, ",", j, ") = ", data(i, j), " expected ", brightness_temperature_data(i, j)
                call exit(1)
            end if
        end do
    end do

    status = nf90_close(ncid)

end program ioda_v1_to_v3_test