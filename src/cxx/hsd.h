#ifndef HSD_H
#define HSD_H
#include <iostream>
#include <sstream>

float calc_solar_zenith_angle(
    float xlat, float xlon, int gmt, int minute, int julian
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
}


#endif //HSD_H
