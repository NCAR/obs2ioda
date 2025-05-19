#ifndef IODA_GROUP_H
#define IODA_GROUP_H
#include <FilePathConfig.h>
#include <netcdf>
#include "ioda_obs_schema/ioda_obs_schema.h"
#include <memory>

class IodaGroup {
public:
    std::string m_name;
    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));
    std::unordered_map<std::string, std::unique_ptr<IodaGroup>> m_groups;

    explicit IodaGroup(const std::string& name) {
        m_schema.addVariableRegexPattern(R"(([a-zA-Z0-9_]+)@)");
        m_schema.addVariableRegexPattern(
            R"(^(.*)_\d+@[a-zA-Z0-9_]+$)");
        m_schema.addGroupRegexPattern(R"(@([a-zA-Z0-9_]+))");
        m_name = m_schema.getGroup(name)->getValidName();
    }
};
#endif // IODA_GROUP_H

