#ifndef IODA_ATTRIBUTE_H
#define IODA_ATTRIBUTE_H

#include <FilePathConfig.h>
#include <netcdf>
#include "ioda_obs_schema/ioda_obs_schema.h"

class IodaAttribute {
public:
    std::string m_name;
    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));

    explicit IodaAttribute(const std::string& name) {
        m_schema.addVariableRegexPattern(R"(([a-zA-Z0-9_]+)@)");
        m_schema.addVariableRegexPattern(
            R"(^(.*)_\d+@[a-zA-Z0-9_]+$)");
        m_schema.addAttributeRegexPattern(R"(@([a-zA-Z0-9_]+))");
        m_name = m_schema.getAttribute(name)->getValidName();
    }
};

#endif // IODA_ATTRIBUTE_H