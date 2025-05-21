#ifndef IODA_VARIABLE_H
#define IODA_VARIABLE_H
#include <FilePathConfig.h>

#include "ioda_obs_schema.h"

class IodaVariable {
    std::string m_name;
    /**< Canonical name of the variable resolved through the schema. */

    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));
    /**< IODA schema used to validate and interpret variable metadata. */

    bool m_isChannelVariable = false;
    /**< Whether this variable represents a channel-specific variable (e.g., ends with _1@). */

    bool m_isV1Variable = false;
    /**< Whether this variable matches the legacy v1 variable format. */

    std::string m_v1VariableRegexPattern = R"(([a-zA-Z0-9_]+)@)";
    /**< Regex pattern used to identify v1 variable names. */

    std::string m_channelVariableRegexPattern = R"(^(.*)_\d+@[a-zA-Z0-9_]+$)";
    /**< Regex pattern used to identify channel variables. */

    std::string m_groupRegexPattern = R"(@([a-zA-Z0-9_]+))";
    /**< Regex pattern used to extract group names from variable names. */

    std::string m_channelIndexRegexPattern = R"(_(\d+)@)";
    /**< Regex pattern to extract channel index from the variable name. */

public:
    /**
     * @brief Construct a new IodaVariable and resolve its metadata from the schema.
     * @param name The name of the variable (may include channel or group syntax).
     */
    explicit IodaVariable(const std::string &name);

    /**
     * @brief Check whether the variable is a channel-specific variable.
     * @return True if the variable includes a channel number suffix.
     */
    [[nodiscard]] bool isChannelVariable() const;

    /**
     * @brief Check whether the variable is in v1 format.
     * @return True if the variable matches the v1 variable regex pattern.
     */
    [[nodiscard]] bool isV1Variable() const;

    /**
     * @brief Get the canonical name of the variable.
     * @return Variable name as resolved by the schema.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * @brief Extract the channel index from the variable name.
     * @param name The full name of the variable including channel.
     * @return Zero-based index of the channel.
     * @throws std::runtime_error if the name is invalid or has no channel index.
     */
    int getChannelIndex(const std::string& name);
};

#endif // IODA_VARIABLE_H
