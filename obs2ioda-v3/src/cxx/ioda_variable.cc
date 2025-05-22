#include "ioda_variable.h"

IodaVariable::IodaVariable(const std::string &name) {
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

bool IodaVariable::isChannelVariable() const {
    return m_isChannelVariable;
}

bool IodaVariable::isV1Variable() const {
    return m_isV1Variable;
}

std::string IodaVariable::getName() const {
    return m_name;
}

int IodaVariable::getChannelIndex(const std::string &name, const std::vector<int>& channels) {
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
        auto it = std::find(channels.begin(), channels.end(), std::stoi(match[1]));
        if (it != channels.end()) {
            return  std::distance(channels.begin(), it);
        }
            throw std::runtime_error("Channel index not found in: " + name);
    }
    throw std::runtime_error("No channel index found in: " + name);
}
