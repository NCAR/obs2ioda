#ifndef IODA_VARIABLE_H
#define IODA_VARIABLE_H
#include <FilePathConfig.h>
#include <netcdf>
#include <ioda_dimension/ioda_dimension.h>

#include "ioda_obs_schema/ioda_obs_schema.h"

class IodaVariable {
    std::string m_name;
    std::unordered_map<std::string, std::shared_ptr<IodaDimension> >
    m_dimensions;
    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));
    bool m_isChannelVariable = false;

public:
    explicit IodaVariable(const std::string &name) {
        m_schema.addVariableRegexPattern(R"(([a-zA-Z0-9_]+)@)");
        m_schema.addVariableRegexPattern(
            R"(^(.*)_\d+@[a-zA-Z0-9_]+$)");
        m_name = m_schema.getVariable(name)->getValidName();
        std::string channelVariableRegexPattern =
            R"(^(.*)_\d+@[a-zA-Z0-9_]+$)";
        std::regex regex(channelVariableRegexPattern);
        std::smatch match;
        if (std::regex_search(name, match, regex)) {
            m_isChannelVariable = true;
        }
    }

    [[nodiscard]] bool isChannelVariable() const {
        return m_isChannelVariable;
    }

    void addDimension(const std::shared_ptr<IodaDimension>& dimension) {
        if (m_dimensions.count(dimension->getName()) == 0) {
            auto validDimensionNames = m_schema.getVariable(m_name)->getDimensions();
            auto validDimensionName = [&validDimensionNames](const std::string& str) {
                return std::any_of(validDimensionNames.begin(), validDimensionNames.end(),
                    [&str](const std::vector<std::string>& innerVec) {
                        return std::find(innerVec.begin(), innerVec.end(), str) != innerVec.end();
                    });
            };
            if (validDimensionName(dimension->getName())) {
                m_dimensions[dimension->getName()] = dimension;
            } else {
                throw std::runtime_error("Invalid dimension name: " + dimension->getName());
            }
        }
    }

    std::shared_ptr<IodaDimension> getDimension(const std::string &name) {
        try {
            return m_dimensions.at(m_schema.getDimension(name)->getValidName());
        } catch (const std::out_of_range &e) {
            throw std::runtime_error("Dimension not found: " + name);
        }
    }

    [[nodiscard]] unsigned long numDimensions() const {
        return m_dimensions.size();
    }

    [[nodiscard]] std::string getName() const {
        return m_name;
    }
};
#endif // IODA_VARIABLE_H
