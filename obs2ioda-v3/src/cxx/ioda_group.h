#ifndef IODA_GROUP_H
#define IODA_GROUP_H
#include <FilePathConfig.h>
#include "ioda_obs_schema.h"
#include "ioda_variable.h"

class IodaGroup {
    std::unordered_map<std::string, std::shared_ptr<IodaGroup> > m_groups;
    /**< Map of subgroup names to IodaGroup instances. */

    std::unordered_map<std::string, std::shared_ptr<IodaVariable> > m_variables;
    /**< Map of variable names to IodaVariable instances. */

    std::string m_name;
    /**< Name of the group. */

    IodaObsSchema m_schema = IodaObsSchema(
        YAML::LoadFile(Obs2Ioda::IODA_SCHEMA_YAML));
    /**< IODA schema used to validate and resolve variable/group names. */

    std::string v1VariableRegexPattern = R"(([a-zA-Z0-9_]+)@)";
    /**< Regex pattern for identifying v1 variable names. */

    std::string channelVariableRegexPattern = R"(^(.*)_\d+@[a-zA-Z0-9_]+$)";
    /**< Regex pattern for identifying channel-based variable names. */

    std::string v1GroupRegexPattern = R"(@([a-zA-Z0-9_]+))";
    /**< Regex pattern for extracting group names from variable names. */

public:
    /**
     * @brief Construct a new IodaGroup using a group name.
     * @param name Group name to resolve using the schema.
     */
    explicit IodaGroup(const std::string &name);

    /**
     * @brief Get the resolved name of the group.
     * @return Reference to the group name.
     */
    [[nodiscard]] const std::string &getName() const;

    /**
     * @brief Access the IODA observation schema used by the group.
     * @return Reference to the internal IodaObsSchema object.
     */
    [[nodiscard]] const IodaObsSchema &getSchema() const;
};

#endif // IODA_GROUP_H
