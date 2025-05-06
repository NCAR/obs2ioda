#include "NetcdfCxxAttributeFixture.h"

void NetcdfCxxAttributeFixture::SetUp() {
    Obs2Ioda::netcdfCreate(
        netcdfAttributeFileName.c_str(), &netcdfAttributeFileID,
        netcdfAttributeFileMode
    );
    getArrayAttributeValue = std::shared_ptr<int>(
        new int[putArrayAttributeValue.size()],
        std::default_delete<int[]>()
    );
    getStringArrayAttributeValue = new char *[numStrings];
    for (size_t i = 0; i < numStrings; ++i) {
        getStringArrayAttributeValue[i] = new char[stringSize];
    }
    getStringAttributeValue = new char[stringSize];
    Obs2Ioda::netcdfAddDim(
        netcdfAttributeFileID, nullptr, dimensionName.c_str(),
        putArrayAttributeValue.size(), &dimID
    );
    Obs2Ioda::netcdfAddGroup(
        netcdfAttributeFileID, nullptr, groupName.c_str()
    );
    Obs2Ioda::netcdfAddVar(
        netcdfAttributeFileID, nullptr, variableName.c_str(), NC_INT, 1,
        dimensionNames.data()
    );
    Obs2Ioda::netcdfAddVar(
        netcdfAttributeFileID, groupName.c_str(), variableName.c_str(),
        NC_INT, 1, dimensionNames.data()
    );
}

void NetcdfCxxAttributeFixture::TearDown() {
    Obs2Ioda::netcdfClose(netcdfAttributeFileID);
    for (size_t i = 0; i < numStrings; ++i) {
        delete[] getStringArrayAttributeValue[i];
    }
    delete[] getStringArrayAttributeValue;
    delete[] getStringAttributeValue;
    if (std::remove(netcdfAttributeFileName.c_str()) != 0) {
        std::cerr << "Error deleting file: " << netcdfAttributeFileName
                << std::endl;
    } else if (std::ifstream(netcdfAttributeFileName)) {
        std::cerr << "File still exists: " << netcdfAttributeFileName <<
                std::endl;
    } else {
    }
}
