module hsd_cxx_i_mod
    use iso_c_binding, only: c_float, c_int
    implicit none

    interface
        function calc_solar_zenith_angle(xlat, xlon, gmt, minute, julian) bind(C, name = "C_calc_solar_zenith_angle")
            use iso_c_binding
            real(c_float)  :: xlat, xlon
            integer(c_int) :: gmt, minute, julian
            real(c_float) :: calc_solar_zenith_angle
        end function
    end interface
end module hsd_cxx_i_mod
