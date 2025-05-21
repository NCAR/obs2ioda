#include "ioda_group.h"

IodaGroup::IodaGroup(const std::string &name) {
    m_schema.addVariableRegexPattern(v1VariableRegexPattern);
    m_schema.addVariableRegexPattern(channelVariableRegexPattern);
    m_schema.addGroupRegexPattern(v1GroupRegexPattern);
    m_name = m_schema.getGroup(name)->getValidName();
}

const std::string & IodaGroup::getName() const {
    return m_name;
}

const std::shared_ptr<IodaObsSchema> IodaGroup::getSchema() const {
    return std::make_shared<IodaObsSchema>(m_schema);
}
