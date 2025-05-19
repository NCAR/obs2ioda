#ifndef IODA_DIMENSION_H
#define IODA_DIMENSION_H
#include <FilePathConfig.h>
#include <netcdf>
#include "ioda_obs_schema/ioda_obs_schema.h"
#include "ioda_dimension/ioda_dimension.h"

class IodaDimension {
public:
    std::string m_name;
    int m_size;
    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));
    IodaDimension(const std::string& name, int size) : m_size(size) {
        m_schema.addVariableRegexPattern(R"(([a-zA-Z0-9_]+)@)");
        m_schema.addVariableRegexPattern(
            R"(^(.*)_\d+@[a-zA-Z0-9_]+$)");
        m_schema.addGroupRegexPattern(R"(@([a-zA-Z0-9_]+))");
        m_name = m_schema.getDimension(name)->getValidName();
    }
};
#endif // IODA_DIMENSION_H
