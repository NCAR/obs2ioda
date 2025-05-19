#ifndef IODA_VARIABLE_H
#define IODA_VARIABLE_H
#include <FilePathConfig.h>
#include <netcdf>
#include "ioda_obs_schema/ioda_obs_schema.h"

class IodaVariable {
public:
    std::string m_name;
    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));

    explicit IodaVariable(const std::string& name) {
        m_schema.addVariableRegexPattern(R"(([a-zA-Z0-9_]+)@)");
        m_schema.addVariableRegexPattern(
            R"(^(.*)_\d+@[a-zA-Z0-9_]+$)");
        m_schema.addGroupRegexPattern(R"(@([a-zA-Z0-9_]+))");
        m_name = m_schema.getVariable(name)->getValidName();
    }
};
#endif // IODA_VARIABLE_H

