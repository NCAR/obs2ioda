#include "missing_value.h"

MissingValueVariant missingValue(const std::string &type,
                                 const std::unordered_map<std::string,
                                     std::function<
                                         MissingValueVariant()> > &
                                 missingValueVariantMap) {
    if (const auto it = missingValueVariantMap.find(type);
        it != missingValueVariantMap.end()) {
        return MissingValueVariant(it->second());
    }
    throw std::invalid_argument("Unknown type: " + type);
}

MissingValueVariant
missingValueFromCdlTypeName(const std::string &type) {
    return missingValue(
        type,
        {
            {"char", [] { return missingValue<char>(); }},
            {"short", [] { return missingValue<int16_t>(); }},
            {"int", [] { return missingValue<int32_t>(); }},
            {"int64", [] { return missingValue<int64_t>(); }},
            {"float", [] { return missingValue<float>(); }},
            {"double", [] { return missingValue<double>(); }},
            {"bool", [] { return missingValue<bool>(); }},
            {"string", [] { return missingValue<std::string>(); }}
        }
    );
}
