#ifndef MISSING_VALUE_H
#define MISSING_VALUE_H
#include <functional>
#include <limits>
#include <string>
#include <variant>

/**
 * @brief Utilities for retrieving standardized missing values across common data types.
 *
 * Provides functions for retrieving type-specific "missing" sentinel values used to represent
 * invalid or absent data. These values are consistent with those defined in OOPS, which IODA relies on.
 *
 * Includes a variant-based interface for dynamic type resolution from CDL type strings.
 *
 * Supported types:
 * - int16_t, int32_t, int64_t
 * - float, double
 * - bool, char, std::string
 */
using MissingValueVariant = std::variant<
    int16_t,
    int32_t,
    int64_t,
    float,
    double,
    bool,
    char,
    std::string
>;
/**
 * @brief Returns the standard missing value for the given type.
 *
 * The returned value is consistent with OOPS missing value conventions,
 * ensuring compatibility across IODA and associated data processing tools.
 *
 * @tparam T The type for which the missing value is requested.
 * @return The missing value for type T.
 *
 * @note Triggers a compile-time error if T is not a supported type.
 */
template<typename T>
constexpr T missingValue();

template<typename T>
constexpr T missingValue() {
    if constexpr (std::is_same_v<T, float>) {
        return std::numeric_limits<float>::lowest() * 0.99f;
    } else if constexpr (std::is_same_v<T, double>) {
        return static_cast<double>(
            std::numeric_limits<float>::lowest() * 0.98f);
    } else if constexpr (std::is_same_v<T, int16_t>) {
        return std::numeric_limits<int16_t>::lowest() + 3;
    } else if constexpr (std::is_same_v<T, int32_t>) {
        return std::numeric_limits<int32_t>::lowest() + 5;
    } else if constexpr (std::is_same_v<T, int64_t>) {
        return std::numeric_limits<int64_t>::lowest() + 7;
    } else if constexpr (std::is_same_v<T, bool>) {
        return false;
    } else if constexpr (std::is_same_v<T, char>) {
        return '\0';
    } else if constexpr (std::is_same_v<T, std::string>) {
        return "*** MISSING ***";
    } else {
        static_assert(sizeof(T) == 0,
                      "missingValue not implemented for this type");
    }
    return T{};
}

/**
 * @brief Retrieves a missing value using a type string and a variant-producing factory map.
 *
 * Looks up the given type string in the supplied map and returns the corresponding
 * `MissingValueVariant` by invoking the associated factory function.
 *
 * @param type The type name as a string (e.g., "float", "int64").
 * @param missingValueVariantMap Map from type names to factory functions returning missing values.
 * @return A variant holding the corresponding missing value.
 *
 * @throws std::invalid_argument If the type name is not found in the map.
 */
MissingValueVariant missingValue(
    const std::string &type,
    const std::unordered_map<std::string, std::function<
        MissingValueVariant()> > &missingValueVariantMap
);

/**
 * @brief Returns the standard missing value for a CDL type string.
 *
 * Maps a CDL-style type name (e.g., "float", "string", "int64") to the
 * corresponding missing value wrapped in a `MissingValueVariant`.
 *
 * @param type The CDL type name.
 * @return A variant holding the missing value for the type.
 *
 * @throws std::invalid_argument If the type name is not recognized.
 */
MissingValueVariant missingValueFromCdlTypeName(
    const std::string &type
);

#endif //MISSING_VALUE_H
