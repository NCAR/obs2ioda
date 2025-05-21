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
    std::string m_v1VariableRegexPattern =
        R"(([a-zA-Z0-9_]+)@)";
    std::string m_channelVariableRegexPattern =
        R"(^(.*)_\d+@[a-zA-Z0-9_]+$)";
    std::string m_groupRegexPattern =
        R"(@([a-zA-Z0-9_]+))";
    std::string m_channelIndexRegexPattern =
        R"(_(\d+)@)";

public:
    explicit IodaVariable(const std::string &name);


    [[nodiscard]] bool isChannelVariable() const;

    [[nodiscard]] bool isV1Variable() const;

    [[nodiscard]] std::string getName() const;

    int getChannelIndex(const std::string& name);
};
#endif // IODA_VARIABLE_H
