#include <gtest/gtest.h>
#include <string>
#include <limits>
#include <variant>
#include <stdexcept>
#include <type_traits>
#include <cstdint>
#include <functional>
#include "missing_value.h"

/**
 * @brief Returns the expected missing value for a given type.
 *
 * This helper is used to validate the values returned by `missingValueFromCdlTypeName`
 * against known expected constants for each supported type. The values mirror those
 * defined in the main implementation and help ensure correctness in unit tests.
 */
template<typename T>
constexpr T expectedMissingValue();

template<>
constexpr float expectedMissingValue<float>() {
    return std::numeric_limits<float>::lowest() * 0.99f;
}

template<>
constexpr double expectedMissingValue<double>() {
    return std::numeric_limits<float>::lowest() * 0.98f;
}

template<>
constexpr int16_t expectedMissingValue<int16_t>() {
    return std::numeric_limits<int16_t>::lowest() + 3;
}

template<>
constexpr int32_t expectedMissingValue<int32_t>() {
    return std::numeric_limits<int32_t>::lowest() + 5;
}

template<>
constexpr int64_t expectedMissingValue<int64_t>() {
    return std::numeric_limits<int64_t>::lowest() + 7;
}

template<>
constexpr bool expectedMissingValue<bool>() {
    return false;
}

template<>
constexpr char expectedMissingValue<char>() {
    return '\0';
}

template<>
std::string expectedMissingValue<std::string>() {
    return "*** MISSING ***";
}

/**
 * @brief Tests variant-based missing value resolution for valid CDL types.
 *
 * This test verifies that:
 * - Each known CDL type string produces the correct variant type.
 * - The value stored in the variant matches the expected missing value constant.
 *
 * CDL type strings tested include: "int", "float", "double", "string", "char",
 * "int64", "short", and "bool". Each call to `missingValueFromCdlTypeName` should
 * return a `std::variant` holding the appropriate type and value.
 */
TEST(missingValueTests, validCdlTypesProduceCorrectVariantsAndMissingValues) {
    auto intVal     = missingValueFromCdlTypeName("int");
    auto floatVal   = missingValueFromCdlTypeName("float");
    auto doubleVal  = missingValueFromCdlTypeName("double");
    auto stringVal  = missingValueFromCdlTypeName("string");
    auto charVal    = missingValueFromCdlTypeName("char");
    auto int64Val   = missingValueFromCdlTypeName("int64");
    auto int16Val   = missingValueFromCdlTypeName("short");
    auto boolVal    = missingValueFromCdlTypeName("bool");

    ASSERT_TRUE(std::holds_alternative<int32_t>(intVal) &&
                std::get<int32_t>(intVal) == expectedMissingValue<int32_t>());
    ASSERT_TRUE(std::holds_alternative<float>(floatVal) &&
                std::get<float>(floatVal) == expectedMissingValue<float>());
    ASSERT_TRUE(std::holds_alternative<double>(doubleVal) &&
                std::get<double>(doubleVal) == expectedMissingValue<double>());
    ASSERT_TRUE(std::holds_alternative<std::string>(stringVal) &&
                std::get<std::string>(stringVal) == expectedMissingValue<std::string>());
    ASSERT_TRUE(std::holds_alternative<char>(charVal) &&
                std::get<char>(charVal) == expectedMissingValue<char>());
    ASSERT_TRUE(std::holds_alternative<int64_t>(int64Val) &&
                std::get<int64_t>(int64Val) == expectedMissingValue<int64_t>());
    ASSERT_TRUE(std::holds_alternative<int16_t>(int16Val) &&
                std::get<int16_t>(int16Val) == expectedMissingValue<int16_t>());
    ASSERT_TRUE(std::holds_alternative<bool>(boolVal) &&
                std::get<bool>(boolVal) == expectedMissingValue<bool>());
}

/**
 * @brief Tests that an invalid CDL type throws an exception.
 *
 * This test ensures that providing an unsupported CDL type string to
 * `missingValueFromCdlTypeName` results in a `std::invalid_argument` exception.
 */
TEST(missingValueTests, invalidCdlTypeThrows) {
    EXPECT_THROW(missingValueFromCdlTypeName("unknown_type"), std::invalid_argument);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

