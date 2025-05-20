#include "ioda_obs_schema_map.h"

#include <ncException.h>


namespace Obs2Ioda {

    IodaObsSchemaMap &IodaObsSchemaMap::getInstance() {
        static IodaObsSchemaMap instance;
        return instance;
    }

    void IodaObsSchemaMap::addIodaObsSchema(const int iodaObsSchemaID,
        const std::shared_ptr<IodaObsSchema> &iodaObsSchema) {
        iodaObsSchema->addVariableRegexPattern(R"(([a-zA-Z0-9_]+)@)");
        iodaObsSchema->addVariableRegexPattern(
            R"(^(.*)_\d+@[a-zA-Z0-9_]+$)");
        iodaObsSchema->addGroupRegexPattern(R"(@([a-zA-Z0-9_]+))");
        m_objectRegistry.addObject(iodaObsSchemaID, iodaObsSchema);
    }

    std::shared_ptr<IodaObsSchema> IodaObsSchemaMap::getIodaObsSchema(const int iodaObsSchemaID) {
        try {
            return m_objectRegistry.getObject(iodaObsSchemaID);
        }
        catch (const std::runtime_error &e) {
            throw netCDF::exceptions::NcBadId(
                ("IodaObsSchema ID not found in the IodaObsSchema map: " +
                 std::to_string(iodaObsSchemaID)).c_str(),
                __FILE__,
                __LINE__
            );
        }
    }

    void IodaObsSchemaMap::removeIodaObsSchema(int iodaObsSchemaID) {
        try {
            m_objectRegistry.removeObject(iodaObsSchemaID);
        }
        catch (const std::runtime_error &e) {
            throw netCDF::exceptions::NcBadId(
                ("IodaObsSchema ID not found in the IodaObsSchema map: " +
                 std::to_string(iodaObsSchemaID)).c_str(),
                __FILE__,
                __LINE__
            );
        }
    }
}
