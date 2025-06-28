#include <cmath>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <numbers>

#include "hsd.h"

#include <sstream>

[[nodiscard]] float calc_solar_zenith_angle(
    const float xlat, const float xlon, const int gmt, const int minute,
    const int julian
) {
    using geo_constants::deg2rad;
    using geo_constants::rad2deg;
    const bool invalid_input =
            gmt < 0 || gmt > 23 || minute < 0 || minute > 59 || julian <
            1 || julian > 366 || std::abs(xlon) > 360.0f || std::abs(
                xlat
            ) > 90.0f;

    if (invalid_input) {
        std::ostringstream err_msg;
        err_msg << "Invalid input values: gmt = " << gmt <<
                ", minute = " << minute << ", julian = " << julian <<
                ", xlat = " << xlat << ", xlon = " << xlon;

        throw std::invalid_argument(err_msg.str());
    }

    const float lat_rad = xlat * deg2rad;
    const auto gmt_f = static_cast<float>(gmt);
    const auto min_f = static_cast<float>(minute);
    const auto julian_f = static_cast<float>(julian);

    constexpr float obliquity = 23.5f;
    constexpr float deg_per_day = 360.0f / 365.0f;

    const float solar_lon = (julian_f >= 80.0f)
                                ? (julian_f - 80.0f) * deg_per_day
                                : (julian_f + 285.0f) * deg_per_day;

    const float declination = std::asin(
        std::sin(obliquity * deg2rad) * std::sin(solar_lon * deg2rad)
    );

    const float day_angle = 2.0f * std::numbers::pi_v<float> * (
                                julian_f - 1.0f) / 365.0f;

    const float equation_of_time =
    (0.000075f + 0.001868f * std::cos(day_angle) - 0.032077f *
     std::sin(day_angle) - 0.014615f * std::cos(2.0f * day_angle) -
     0.04089f * std::sin(2.0f * day_angle)) * 229.18f;

    const float solar_time = gmt_f + (min_f + equation_of_time) / 60.0f;
    const float local_time = solar_time + xlon / 15.0f;
    const float hour_angle = 15.0f * (local_time - 12.0f) * deg2rad;

    const float cos_zenith = std::sin(lat_rad) * std::sin(declination) +
                             std::cos(lat_rad) * std::cos(declination) *
                             std::cos(hour_angle);

    const float zenith_angle = std::acos(
                                   std::clamp(cos_zenith, -1.0f, 1.0f)
                               ) * rad2deg;

    return zenith_angle;
}


void pixlin_to_lonlat(
    const int pix, const int lin, const double sub_lon, const int cfac,
    const int lfac, const float coff, const float loff,
    const double sat_dis, const double eqtr_radius,
    const double polr_radius, const double proj_param3,
    const double proj_param_sd, double &lon, double &lat,
    int &valid_pixel
) {
    using geo_constants::deg2rad;
    using geo_constants::rad2deg;
    using geo_constants::sclunit;
    // === Input validation ===
    valid_pixel = 0;

    if (pix < 0 || lin < 0) {
        throw std::invalid_argument("pix and lin must be non-negative");
    }
    if (cfac <= 0 || lfac <= 0) {
        throw std::invalid_argument("cfac and lfac must be positive");
    }
    if (sat_dis <= 0 || sat_dis < eqtr_radius) {
        throw std::invalid_argument(
            "sat_dis must be greater than Earth's radius and positive"
        );
    }
    if (eqtr_radius < 6000.0 || eqtr_radius > 7000.0) {
        throw std::invalid_argument(
            "eqtr_radius is out of realistic range (6000–7000 km)"
        );
    }
    if (polr_radius < 6000.0 || polr_radius > 7000.0) {
        throw std::invalid_argument(
            "polr_radius is out of realistic range (6000–7000 km)"
        );
    }
    if (proj_param3 < 1.0 || proj_param3 > 1.01) {
        throw std::invalid_argument(
            "proj_param3 is out of expected range (near 1.0067)"
        );
    }
    if (proj_param_sd <= 0.0) {
        throw std::invalid_argument("proj_param_sd must be positive");
    }

    // === Intermediate coordinates ===
    const auto c = static_cast<double>(pix);
    const auto l = static_cast<double>(lin);

    const double x = deg2rad * (c - static_cast<double>(coff)) / (
                         sclunit * static_cast<double>(cfac));
    const double y = deg2rad * (l - static_cast<double>(loff)) / (
                         sclunit * static_cast<double>(lfac));

    const double cos_x = std::cos(x);
    const double cos_y = std::cos(y);
    const double sin_y = std::sin(y);

    const double term1 = sat_dis * cos_x * cos_y;
    const double term2 = cos_y * cos_y + proj_param3 * sin_y * sin_y;
    const double sd_squared = term1 * term1 - term2 * proj_param_sd;

    if (sd_squared < 0.0 || !std::isfinite(sd_squared)) {
        return;
    }

    const double sd = std::sqrt(sd_squared);
    const double sn = (term1 - sd) / term2;

    const double s1 = sat_dis - sn * cos_x * cos_y;
    const double s2 = sn * std::sin(x) * cos_y;
    const double s3 = -sn * sin_y;
    const double sxy = std::sqrt(s1 * s1 + s2 * s2);

    lon = rad2deg * std::atan2(s2, s1) + sub_lon;
    lat = rad2deg * std::atan(proj_param3 * s3 / sxy);

    if (lon > 180.0) lon -= 360.0;
    if (lon < -180.0) lon += 360.0;

    valid_pixel = 1;
}
