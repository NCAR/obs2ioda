#ifndef IODA_GROUP_H
#define IODA_GROUP_H
#include <FilePathConfig.h>
#include <netcdf>
#include "ioda_obs_schema/ioda_obs_schema.h"
#include "ioda_dimension/ioda_dimension.h"
#include "ioda_variable/ioda_variable.h"

class IodaGroup {
    std::unordered_map<std::string, std::shared_ptr<IodaGroup> >
    m_groups;
    std::unordered_map<std::string, std::shared_ptr<IodaDimension> >
    m_dimensions;
    std::unordered_map<std::string, std::shared_ptr<IodaVariable> >
    m_variables;
    std::string m_name;
    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));

public:

    explicit IodaGroup(const std::string &name) {
        m_schema.addGroupRegexPattern(R"(@([a-zA-Z0-9_]+))");
        m_name = m_schema.getGroup(name)->getValidName();
    }

    void addGroup(const std::shared_ptr<IodaGroup>& group) {
        if (m_groups.count(group->m_name) == 0) {
            m_groups[group->m_name] = group;
        }
    }
    [[nodiscard]] unsigned long numGroups() const {
        return m_groups.size();
    }
    std::shared_ptr<IodaGroup> getGroup(const std::string &name) {
        try {
            return m_groups.at(m_schema.getGroup(name)->getValidName());
        } catch (const std::out_of_range &e) {
            throw std::runtime_error("Group not found: " + name);
        }
    }
    void addDimension(const std::shared_ptr<IodaDimension>& dimension) {
        if (m_dimensions.count(dimension->getName()) == 0) {
            m_dimensions[dimension->getName()] = dimension;
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
    void addVariable(const std::shared_ptr<IodaVariable>& variable) {
        if (m_variables.count(variable->getName()) == 0) {
            m_variables[variable->getName()] = variable;
            if (variable->isChannelVariable()) {
                getDimension("nchans")->setSize(1);
            }
        }
        else if (variable->isChannelVariable()) {
            getDimension("nchans")->setSize(
                getDimension("nchans")->getSize() + 1);
        }
    }
    std::shared_ptr<IodaVariable> getVariable(const std::string &name) {
        try {
            return m_variables.at(m_schema.getVariable(name)->getValidName());
        } catch (const std::out_of_range &e) {
            throw std::runtime_error("Variable not found: " + name);
        }
    }
    [[nodiscard]] unsigned long numVariables() const {
        return m_variables.size();
    }

    [[nodiscard]] const std::string &getName() const {
        return m_name;
    }
};
#endif // IODA_GROUP_H
