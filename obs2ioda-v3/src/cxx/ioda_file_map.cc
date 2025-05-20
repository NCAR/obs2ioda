#include "ioda_file_map.h"
#include "ioda_group.h"


#include <ncException.h>


namespace Obs2Ioda {

    IodaFileMap &IodaFileMap::getInstance() {
        static IodaFileMap instance;
        return instance;
    }

    void IodaFileMap::addIodaFile(int iodaFileID,
        const std::shared_ptr<IodaGroup> &iodaFile) {
        iodaFileMap[iodaFileID] = iodaFile;
    }

    std::shared_ptr<IodaGroup> IodaFileMap::getIodaFile(const int iodaFileID) {
        const auto it = iodaFileMap.find(iodaFileID);
        if (it == iodaFileMap.end()) {
            throw netCDF::exceptions::NcBadId(
                ("IodaFile ID not found in the IodaFile map: " +
                 std::to_string(iodaFileID)).c_str(),
               __FILE__,
               __LINE__
           );
        }
        return it->second;
    }

    void IodaFileMap::removeIodaFile(int iodaFileID) {
        auto iodaFileIterator = this->iodaFileMap.find(iodaFileID);
        if (iodaFileIterator == this->iodaFileMap.end()) {
            throw netCDF::exceptions::NcBadId(
                ("IodaFile ID not found in the IodaFile map: " +
                 std::to_string(iodaFileID)).c_str(),
               __FILE__,
               __LINE__
           );
        }
        this->iodaFileMap.erase(iodaFileIterator);
    }
}
