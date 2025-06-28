module hsd_cxx_i_mod
    use iso_c_binding, only: c_float, c_int, c_double, c_long
    implicit none

    interface
        function calc_solar_zenith_angle(xlat, xlon, gmt, minute, julian) bind(C, name = "C_calc_solar_zenith_angle")
            use iso_c_binding
            real(c_float) :: xlat, xlon
            integer(c_int) :: gmt, minute, julian
            real(c_float) :: calc_solar_zenith_angle
        end function

        subroutine pixlin_to_lonlat(pix, lin, sub_lon, cfac, lfac, coff, loff, &
                sat_dis, eqtr_radius, polr_radius, proj_param3, proj_param_sd, &
                lon, lat, valid_pixel) bind(C, name = "C_pixlin_to_lonlat")
            use iso_c_binding
            implicit none
            integer(c_int) :: pix, lin
            real(c_double) :: sub_lon, sat_dis, eqtr_radius, polr_radius
            integer(c_int) :: cfac, lfac
            real(c_float) :: coff, loff
            real(c_double) :: proj_param3, proj_param_sd
            real(c_double) :: lon, lat  ! intent(out) from Fortran perspective
            integer(c_int) :: valid_pixel  ! intent(out) from Fortran perspective
        end subroutine pixlin_to_lonlat
    end interface
end module hsd_cxx_i_mod
