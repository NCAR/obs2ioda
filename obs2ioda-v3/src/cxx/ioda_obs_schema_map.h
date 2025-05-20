#ifndef OBS2IODA_IODA_OBS_SCHEMA_FILE_MAP_H
#define OBS2IODA_IODA_OBS_SCHEMA_FILE_MAP_H

#include "object_registry.h"

#include "ioda_obs_schema.h"

namespace Obs2Ioda {
    class IodaObsSchemaMap {
    public:
        static IodaObsSchemaMap &getInstance();

        IodaObsSchemaMap(
            const IodaObsSchemaMap &
        ) = delete;

        IodaObsSchemaMap &operator=(
            const IodaObsSchemaMap &
        ) = delete;

        void addIodaObsSchema(
            int iodaObsSchemaID,
            const std::shared_ptr<IodaObsSchema> &iodaObsSchema
        );

        std::shared_ptr<IodaObsSchema> getIodaObsSchema(
            int iodaObsSchemaID
        );

        void removeIodaObsSchema(
            int iodaObsSchemaID
        );

    private:
        IodaObsSchemaMap() = default;

        ObjectRegistry<IodaObsSchema> m_objectRegistry;
    };
} // namespace Obs2Ioda

#endif // OBS2IODA_IODA_OBS_SCHEMA_FILE_MAP_H
