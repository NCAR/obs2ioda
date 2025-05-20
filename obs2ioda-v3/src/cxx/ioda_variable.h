#ifndef IODA_VARIABLE_H
#define IODA_VARIABLE_H
#include <FilePathConfig.h>

#include "ioda_obs_schema.h"

class IodaVariable {
    std::string m_name;
    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));
    bool m_isChannelVariable = false;
    bool m_isV1Variable = false;
    int m_numChannels = 0;
    std::string m_v1VariableRegexPattern =
        R"(([a-zA-Z0-9_]+)@)";
    std::string m_channelVariableRegexPattern =
        R"(^(.*)_\d+@[a-zA-Z0-9_]+$)";
    std::string m_groupRegexPattern =
        R"(@([a-zA-Z0-9_]+))";
    std::string m_channelIndexRegexPattern =
        R"(_(\d+)@)";

public:
    explicit IodaVariable(const std::string &name) {
        m_schema.addVariableRegexPattern(m_v1VariableRegexPattern);
        m_schema.addVariableRegexPattern(m_channelVariableRegexPattern);
        m_schema.addGroupRegexPattern(m_groupRegexPattern);

        m_name = m_schema.getVariable(name)->getValidName();

        auto matchesRegex = [&](const std::string &pattern) {
            return std::regex_search(name, std::regex(pattern));
        };

        m_isChannelVariable = matchesRegex(m_channelVariableRegexPattern);
        m_isV1Variable = matchesRegex(m_v1VariableRegexPattern);
    }


    [[nodiscard]] bool isChannelVariable() const {
        return m_isChannelVariable;
    }

    [[nodiscard]] bool isV1Variable() const {
        return m_isV1Variable;
    }

    [[nodiscard]] std::string getName() const {
        return m_name;
    }

    void setNumChannels(const int numChannels) {
        m_numChannels = numChannels;
    }

    [[nodiscard]] int getNumChannels() const {
        return m_numChannels;
    }

    int getChannelIndex(const std::string& name) {
        if (m_name != m_schema.getVariable(name)->getValidName()) {
            throw std::runtime_error("Invalid variable name: " + name);
        }
        if (!isChannelVariable()) {
            throw std::runtime_error("Not a channel variable: " + name);
        }
        // Use regex to get the integer after _ and before @
        std::regex regex(m_channelIndexRegexPattern);
        std::smatch match;
        if (std::regex_search(name, match, regex)) {
            return std::stoi(match[1].str()) - 1; // Convert to zero-based index
        }
        throw std::runtime_error("No channel index found in: " + name);
    }
};
#endif // IODA_VARIABLE_H
