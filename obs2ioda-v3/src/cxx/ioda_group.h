#ifndef IODA_GROUP_H
#define IODA_GROUP_H
#include <FilePathConfig.h>
#include "ioda_obs_schema.h"
#include "ioda_variable.h"

class IodaGroup {
    std::unordered_map<std::string, std::shared_ptr<IodaGroup> >
    m_groups;
    std::unordered_map<std::string, std::shared_ptr<IodaVariable> >
    m_variables;
    std::string m_name;
    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));
    std::string v1VariableRegexPattern =
        R"(([a-zA-Z0-9_]+)@)";
    std::string channelVariableRegexPattern =
        R"(^(.*)_\d+@[a-zA-Z0-9_]+$)";
    std::string v1GroupRegexPattern =
        R"(@([a-zA-Z0-9_]+))";

public:

    explicit IodaGroup(const std::string &name);

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] const IodaObsSchema &getSchema() const;
};
#endif // IODA_GROUP_H
