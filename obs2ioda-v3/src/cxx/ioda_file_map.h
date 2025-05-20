
#ifndef IODA_FILE_MAP_H
#define IODA_FILE_MAP_H
#include <unordered_map>
#include <memory>
#include "ioda_group.h"
namespace Obs2Ioda {
    class IodaFileMap {
    public:
        static IodaFileMap &getInstance();

        IodaFileMap(
            const IodaFileMap &
        ) = delete;

        IodaFileMap &operator=(
            const IodaFileMap &
        ) = delete;

        void addIodaFile(
            int iodaFileID,
            const std::shared_ptr<IodaGroup> &iodaFile
        );

        std::shared_ptr<IodaGroup> getIodaFile(
            int iodaFileID
        );

        void removeIodaFile(
            int iodaFileID
        );

    private:
        IodaFileMap() = default;

        std::unordered_map<int, std::shared_ptr<IodaGroup> >
        iodaFileMap;
    };
} // namespace Obs2Ioda

#endif //IODA_FILE_MAP_H
