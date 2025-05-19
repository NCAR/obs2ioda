#ifndef IODA_DIMENSION_H
#define IODA_DIMENSION_H
#include <FilePathConfig.h>
#include <netcdf>
#include "ioda_obs_schema/ioda_obs_schema.h"
#include "ioda_dimension/ioda_dimension.h"

class IodaDimension {
    std::string m_name;
    int m_size;
    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));
public:

    IodaDimension(const std::string &name,
                  int size) : m_size(size) {
        m_name = m_schema.getDimension(name)->getValidName();
    }

    [[nodiscard]] std::string getName() const {
        return m_name;
    }
    void setSize(int size) {
        m_size = size;
    }
    [[nodiscard]] int getSize() const {
        return m_size;
    }
};
#endif // IODA_DIMENSION_H
