#ifndef HSD_H
#define HSD_H
#include <iostream>
#include <sstream>

namespace geo_constants {
    constexpr float deg2rad = std::numbers::pi_v<float> / 180.0f;
    constexpr float rad2deg = 180.0f / std::numbers::pi_v<float>;
    constexpr double sclunit = std::pow(2.0, -16);
}

float calc_solar_zenith_angle(
    float xlat, float xlon, int gmt, int minute, int julian
);

void pixlin_to_lonlat(
    int pix, int lin, double sub_lon, int cfac, int lfac, float coff,
    float loff, double sat_dis, double eqtr_radius, double polr_radius,
    double proj_param3, double proj_param_sd, double &lon, double &lat,
    int &valid_pixel
);

extern "C" {
float C_calc_solar_zenith_angle(
    const float *xlat, const float *xlon, const int *gmt,
    const int *minute, const int *julian
) {
    try {
        return calc_solar_zenith_angle(
            *xlat, *xlon, *gmt, *minute, *julian
        );
    } catch (const std::invalid_argument &e) {
        std::ostringstream err_msg;
        err_msg << "Invalid argument in C_calc_solar_zenith_angle: " <<
                e.what();
        std::cout << err_msg.str() << std::endl;
        return std::numeric_limits<float>::quiet_NaN();
    }
}

void C_pixlin_to_lonlat(
    const int *pix, const int *lin, const double *sub_lon,
    const int *cfac, const int *lfac, const float *coff,
    const float *loff, const double *sat_dis, const double *eqtr_radius,
    const double *polr_radius, const double *proj_param3,
    const double *proj_param_sd, double *lon, double *lat,
    int *valid_pixel
) {
    try {
        pixlin_to_lonlat(
            *pix, *lin, *sub_lon, *cfac, *lfac, *coff, *loff, *sat_dis,
            *eqtr_radius, *polr_radius, *proj_param3, *proj_param_sd,
            *lon, *lat, *valid_pixel
        );
    } catch (const std::exception &e) {
        std::ostringstream err_msg;
        err_msg << "Error in C_pixlin_to_lonlat: " << e.what();
        std::cout << err_msg.str() << std::endl;
        *lon = std::numeric_limits<double>::quiet_NaN();
        *lat = std::numeric_limits<double>::quiet_NaN();
    }
}
}


#endif //HSD_H
