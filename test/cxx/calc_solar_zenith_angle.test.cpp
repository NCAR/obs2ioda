#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>
#include "hsd.h"  // calc_solar_zenith_angle declaration

constexpr float tolerance = 3.0f;  // degrees, based on model approximation

// Sun directly overhead at equator on March 20, true solar noon
TEST(solar_zenith_angle_test, equator_at_equinox_noon) {
    float angle = calc_solar_zenith_angle(0.0f, 0.0f, 12, 8, 79);  // 12:08 GMT ≈ solar noon
    EXPECT_NEAR(angle, 0.0f, tolerance);
}

// Sun just rising/setting at North Pole on equinox (sun circles horizon)
TEST(solar_zenith_angle_test, north_pole_at_equinox) {
    float angle = calc_solar_zenith_angle(90.0f, 0.0f, 12, 0, 80);
    EXPECT_NEAR(angle, 90.0f, tolerance);
}

// Morning sun at mid-latitude in summer
TEST(solar_zenith_angle_test, midlatitude_morning_summer) {
    float angle = calc_solar_zenith_angle(40.0f, -105.0f, 15, 0, 172);  // 9:00 AM MDT
    EXPECT_GT(angle, 0.0f);
    EXPECT_LT(angle, 90.0f);
}

// Midnight at mid-latitude — sun below horizon
TEST(solar_zenith_angle_test, midlatitude_midnight_dark) {
    float angle = calc_solar_zenith_angle(40.0f, -105.0f, 6, 0, 172);  // 12 AM MDT
    EXPECT_GT(angle, 90.0f);
}


// Southern Hemisphere at equinox, noon
TEST(solar_zenith_angle_test, equator_south_latitude_equinox_noon) {
    float angle_north = calc_solar_zenith_angle(10.0f, 0.0f, 12, 8, 80);
    float angle_south = calc_solar_zenith_angle(-10.0f, 0.0f, 12, 8, 79);
    EXPECT_NEAR(angle_north, angle_south, tolerance);  // should be symmetric
}

// Solstice noon at Tropic of Cancer — sun should be near zenith
TEST(solar_zenith_angle_test, tropic_of_cancer_summer_solstice_noon) {
    float angle = calc_solar_zenith_angle(23.44f, 0.0f, 12, 0, 172);
    EXPECT_NEAR(angle, 0.0f, tolerance);
}

// Solstice noon at Tropic of Capricorn — sun should be near zenith
TEST(solar_zenith_angle_test, tropic_of_capricorn_winter_solstice_noon) {
    float angle = calc_solar_zenith_angle(-23.44f, 0.0f, 12, 0, 355);
    EXPECT_NEAR(angle, 0.0f, tolerance);
}

// High-latitude summer — sun should not set (zenith angle < 90)
TEST(solar_zenith_angle_test, polar_day_north) {
    float angle = calc_solar_zenith_angle(70.0f, 0.0f, 0, 0, 172);  // midnight sun
    EXPECT_LT(angle, 90.0f);
}

// High-latitude winter — sun never rises (zenith angle > 90)
TEST(solar_zenith_angle_test, polar_night_north) {
    float angle = calc_solar_zenith_angle(70.0f, 0.0f, 12, 0, 355);  // deep winter
    EXPECT_GT(angle, 90.0f);
}

// Sun due east (morning) — zenith angle should be ~90
TEST(solar_zenith_angle_test, sun_due_east) {
    float angle = calc_solar_zenith_angle(0.0f, 0.0f, 6, 0, 79);
    EXPECT_NEAR(angle, 90.0f, tolerance);
}

// Sun due west (evening) — zenith angle should be ~90
TEST(solar_zenith_angle_test, sun_due_west) {
    float angle = calc_solar_zenith_angle(0.0f, 0.0f, 18, 0, 80);
    EXPECT_NEAR(angle, 90.0f, tolerance);
}

// Before and after solar noon — angle should be symmetric
TEST(solar_zenith_angle_test, symmetric_around_noon) {
    float angle_before = calc_solar_zenith_angle(0.0f, 0.0f, 11, 8, 80);
    float angle_after  = calc_solar_zenith_angle(0.0f, 0.0f, 13, 8, 79);
    EXPECT_NEAR(angle_before, angle_after, tolerance);
}


TEST(solar_zenith_angle_test, invalid_input_throws) {
    EXPECT_THROW(calc_solar_zenith_angle(95.0f, 0.0f, 12, 0, 100), std::invalid_argument);  // latitude too large
    EXPECT_THROW(calc_solar_zenith_angle(0.0f, 400.0f, 12, 0, 100), std::invalid_argument); // longitude too large
    EXPECT_THROW(calc_solar_zenith_angle(0.0f, 0.0f, 25, 0, 100), std::invalid_argument);   // invalid hour
    EXPECT_THROW(calc_solar_zenith_angle(0.0f, 0.0f, 12, 60, 100), std::invalid_argument);  // invalid minute
    EXPECT_THROW(calc_solar_zenith_angle(0.0f, 0.0f, 12, 0, 0), std::invalid_argument);     // invalid Julian day
    EXPECT_THROW(calc_solar_zenith_angle(0.0f, 0.0f, 12, 0, 367), std::invalid_argument);     // invalid Julian day
}
