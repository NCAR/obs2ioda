#include "ioda_obs_schema_map.h"

#include <ncException.h>


namespace Obs2Ioda {

    IodaObsSchemaMap &IodaObsSchemaMap::getInstance() {
        static IodaObsSchemaMap instance;
        return instance;
    }

    void IodaObsSchemaMap::addIodaObsSchema(int iodaObsSchemaID,
        const std::shared_ptr<IodaObsSchema> &iodaObsSchema) {
        iodaObsSchema->addVariableRegexPattern(R"(([a-zA-Z0-9_]+)@)");
        iodaObsSchema->addVariableRegexPattern(
            R"(^(.*)_\d+@[a-zA-Z0-9_]+$)");
        iodaObsSchema->addGroupRegexPattern(R"(@([a-zA-Z0-9_]+))");
        iodaObsSchemaMap[iodaObsSchemaID] = iodaObsSchema;
    }

    std::shared_ptr<IodaObsSchema> IodaObsSchemaMap::getIodaObsSchema(const int iodaObsSchemaID) {
        const auto it = iodaObsSchemaMap.find(iodaObsSchemaID);
        if (it == iodaObsSchemaMap.end()) {
            throw netCDF::exceptions::NcBadId(
                ("IodaObsSchema ID not found in the IodaObsSchema map: " +
                 std::to_string(iodaObsSchemaID)).c_str(),
               __FILE__,
               __LINE__
           );
        }
        return it->second;
    }

    void IodaObsSchemaMap::removeIodaObsSchema(int iodaObsSchemaID) {
        auto iodaObsSchemaIterator = this->iodaObsSchemaMap.find(iodaObsSchemaID);
        if (iodaObsSchemaIterator == this->iodaObsSchemaMap.end()) {
            throw netCDF::exceptions::NcBadId(
                ("IodaObsSchema ID not found in the IodaObsSchema map: " +
                 std::to_string(iodaObsSchemaID)).c_str(),
               __FILE__,
               __LINE__
           );
        }
        this->iodaObsSchemaMap.erase(iodaObsSchemaIterator);
    }
}
