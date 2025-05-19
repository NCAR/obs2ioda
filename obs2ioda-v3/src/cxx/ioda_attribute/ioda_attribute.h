#ifndef IODA_ATTRIBUTE_H
#define IODA_ATTRIBUTE_H

#include <FilePathConfig.h>
#include <netcdf>
#include "ioda_obs_schema/ioda_obs_schema.h"

class IodaAttribute {
    std::string m_name;
public:
    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));

    explicit IodaAttribute(const std::string& name) {
        m_name = m_schema.getAttribute(name)->getValidName();
    }
    [[nodiscard]] std::string getName() const {
        return m_name;
    }
};

#endif // IODA_ATTRIBUTE_H