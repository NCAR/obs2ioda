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

    explicit IodaGroup(const std::string &name) {
        m_schema.addVariableRegexPattern(v1VariableRegexPattern);
        m_schema.addVariableRegexPattern(channelVariableRegexPattern);
        m_schema.addGroupRegexPattern(v1GroupRegexPattern);
        m_name = m_schema.getGroup(name)->getValidName();
    }

    void addGroup(const std::shared_ptr<IodaGroup>& group) {
        if (m_groups.count(group->m_name) == 0) {
            m_groups[group->m_name] = group;
        }
    }
    [[nodiscard]] unsigned long getNumGroups() const {
        return m_groups.size();
    }
    std::shared_ptr<IodaGroup> getGroup(const std::string &name) {
        try {
            return m_groups.at(m_schema.getGroup(name)->getValidName());
        } catch (const std::out_of_range &e) {
            throw std::runtime_error("Group not found: " + name);
        }
    }

    void addVariable(const std::shared_ptr<IodaVariable>& variable) {
        if (m_variables.count(variable->getName()) == 0) {
            m_variables[variable->getName()] = variable;
        }
        if (variable->isChannelVariable()) {
            m_variables[variable->getName()]->setNumChannels(
                m_variables[variable->getName()]->getNumChannels() + 1);
        }
    }

    std::shared_ptr<IodaVariable> getVariable(const std::string &name) {
        try {
            return m_variables.at(m_schema.getVariable(name)->getValidName());
        } catch (const std::out_of_range &e) {
            throw std::runtime_error("Variable not found: " + name);
        }
    }
    [[nodiscard]] unsigned long getNumVariables() const {
        return m_variables.size();
    }

    [[nodiscard]] const std::string &getName() const {
        return m_name;
    }

    [[nodiscard]] const IodaObsSchema &getSchema() const {
        return m_schema;
    }
    [[nodiscard]] const std::unordered_map<std::string,
        std::shared_ptr<IodaGroup>> &getGroups() const {
        return m_groups;
    }

    [[nodiscard]] const std::unordered_map<std::string,
        std::shared_ptr<IodaVariable>> &getVariables() const {
        return m_variables;
    }
};
#endif // IODA_GROUP_H
