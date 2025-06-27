#include <cmath>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <numbers>

#include "hsd.h"

#include <sstream>

constexpr float deg2rad = std::numbers::pi_v<float> / 180.0f;
constexpr float rad2deg = 180.0f / std::numbers::pi_v<float>;

[[nodiscard]]
float calc_solar_zenith_angle(const float xlat, const float xlon, const int gmt, const int minute, const int julian) {
    const bool invalid_input =
        gmt < 0 || gmt > 23 ||
        minute < 0 || minute > 59 ||
        julian < 1 || julian > 366 ||
        std::abs(xlon) > 360.0f ||
        std::abs(xlat) > 90.0f;

    if (invalid_input) {
        std::ostringstream err_msg;
        err_msg << "Invalid input values: gmt = " << gmt
            << ", minute = " << minute
            << ", julian = " << julian
            << ", xlat = " << xlat
            << ", xlon = " << xlon;

        throw std::invalid_argument(err_msg.str());
    }

    const float lat_rad = xlat * deg2rad;
    const auto gmt_f   = static_cast<float>(gmt);
    const auto min_f   = static_cast<float>(minute);
    const auto julian_f = static_cast<float>(julian);

    constexpr float obliquity = 23.5f;
    constexpr float deg_per_day = 360.0f / 365.0f;

    const float solar_lon = (julian_f >= 80.0f)
        ? (julian_f - 80.0f) * deg_per_day
        : (julian_f + 285.0f) * deg_per_day;

    const float declination = std::asin(
        std::sin(obliquity * deg2rad) * std::sin(solar_lon * deg2rad)
    );

    const float day_angle = 2.0f * std::numbers::pi_v<float> * (julian_f - 1.0f) / 365.0f;

    const float equation_of_time = (0.000075f + 0.001868f * std::cos(day_angle)
        - 0.032077f * std::sin(day_angle)
        - 0.014615f * std::cos(2.0f * day_angle)
        - 0.04089f * std::sin(2.0f * day_angle)) * 229.18f;

    const float solar_time = gmt_f + (min_f + equation_of_time) / 60.0f;
    const float local_time = solar_time + xlon / 15.0f;
    const float hour_angle = 15.0f * (local_time - 12.0f) * deg2rad;

    const float cos_zenith = std::sin(lat_rad) * std::sin(declination) +
                              std::cos(lat_rad) * std::cos(declination) * std::cos(hour_angle);

    const float zenith_angle = std::acos(std::clamp(cos_zenith, -1.0f, 1.0f)) * rad2deg;

    return zenith_angle;
}

