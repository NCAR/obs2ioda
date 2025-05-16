#include "ioda_obs_schema_map.h"


namespace Obs2Ioda {

    IodaObsSchemaMap &IodaObsSchemaMap::getInstance() {
        static IodaObsSchemaMap instance;
        return instance;
    }

    void IodaObsSchemaMap::addIodaObsSchema(int iodaObsSchemaID,
        const std::shared_ptr<IodaObsSchema> &iodaObsSchema) {
        iodaObsSchemaMap[iodaObsSchemaID] = iodaObsSchema;
    }

    std::shared_ptr<IodaObsSchema> IodaObsSchemaMap::getIodaObsSchema(const int iodaObsSchemaID) {
        const auto it = iodaObsSchemaMap.find(iodaObsSchemaID);
        if (it == iodaObsSchemaMap.end()) {
            throw std::runtime_error(
                "IodaObsSchema ID not found in the IodaObsSchema map: " +
                std::to_string(iodaObsSchemaID)
            );
        }
        return it->second;
    }

    void IodaObsSchemaMap::removeIodaObsSchema(int iodaObsSchemaID) {
        auto iodaObsSchemaIterator = this->iodaObsSchemaMap.find(iodaObsSchemaID);
        if (iodaObsSchemaIterator == this->iodaObsSchemaMap.end()) {
            throw std::runtime_error(
                "IodaObsSchema ID not found in the IodaObsSchema map: " +
                std::to_string(iodaObsSchemaID)
            );
        }
        this->iodaObsSchemaMap.erase(iodaObsSchemaIterator);
    }
}
