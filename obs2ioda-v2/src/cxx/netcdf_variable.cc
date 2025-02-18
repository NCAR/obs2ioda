#include "netcdf_variable.h"

#include <ioda_names.h>

#include "netcdf_file.h"
#include "netcdf_error.h"

namespace Obs2Ioda {
    int netcdfAddVar(
        int netcdfID,
        const char *groupName,
        const char *varName,
        nc_type netcdfDataType,
        int numDims,
        const char **dimNames
    ) {
        try {
            auto file = FileMap::getInstance().getFile(netcdfID);
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName));
            std::vector<netCDF::NcDim> dims;
            dims.reserve(numDims);
            for (int i = 0; i < numDims; i++) {
                auto iodaDimName = getIodaName(
                    dimNames[i],
                    IODA_DIMENSION_NAMES
                );
                dims.push_back(file->getDim(iodaDimName));;
            }
            auto iodaVarName = getIodaName(
                varName,
                IODA_VARIABLE_NAMES
            );
            auto var = group->addVar(
                iodaVarName,
                netCDF::NcType(netcdfDataType),
                dims
            );
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }

    template<typename T>
    int netcdfPutVar(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const T *values
    ) {
        try {
            auto file = FileMap::getInstance().getFile(netcdfID);
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName));
            auto iodaVarName = getIodaName(
                varName,
                IODA_VARIABLE_NAMES
            );
            auto var = group->getVar(iodaVarName);
            var.putVar(values);
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }

    int netcdfPutVarInt(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const int *values
    ) {
        return netcdfPutVar(
            netcdfID,
            groupName,
            varName,
            values
        );
    }

    int netcdfPutVarInt64(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const long long *values
    ) {
        return netcdfPutVar(
            netcdfID,
            groupName,
            varName,
            values
        );
    }

    int netcdfPutVarReal(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const float *values
    ) {
        return netcdfPutVar(
            netcdfID,
            groupName,
            varName,
            values
        );
    }

    int netcdfPutVarString(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const char **values
    ) {
        return netcdfPutVar(
            netcdfID,
            groupName,
            varName,
            values
        );
    }

    template<typename T>
    int netcdfSetFill(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        T fillValue
    ) {
        try {
            auto file = FileMap::getInstance().getFile(netcdfID);
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName));
            auto iodaVarName = getIodaName(
                varName,
                IODA_VARIABLE_NAMES
            );
            auto var = group->getVar(iodaVarName);
            var.setFill(
                fillMode,
                fillValue
            );
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }

    int netcdfSetFillInt(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        int fillValue
    ) {
        return netcdfSetFill(
            netcdfID,
            groupName,
            varName,
            fillMode,
            fillValue
        );
    }

    int netcdfSetFillReal(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        float fillValue
    ) {
        return netcdfSetFill(
            netcdfID,
            groupName,
            varName,
            fillMode,
            fillValue
        );
    }

    int netcdfSetFillInt64(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        long long fillValue
    ) {
        return netcdfSetFill(
            netcdfID,
            groupName,
            varName,
            fillMode,
            fillValue
        );
    }

    int netcdfSetFillString(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        const char *fillValue
    ) {
        return netcdfSetFill(
            netcdfID,
            groupName,
            varName,
            fillMode,
            fillValue

        );
    }

    int netcdfGetVarSize(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int *varSize
    ) {
        try {
            auto file = FileMap::getInstance().getFile(
                netcdfID
            );
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName
                                       )
                                   );
            auto iodaVarName = getIodaName(
                varName,
                IODA_VARIABLE_NAMES
            );
            auto var = group->getVar(
                iodaVarName
            );
            *varSize = static_cast<int>(var.getDim(
                0
            ).getSize());
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }

    template<typename T>
    int netcdfGetVar(
        int netcdfID,
        const char *groupName,
        const char *varName,
        T **data
    ) {
        try {
            auto file = FileMap::getInstance().getFile(
                netcdfID
            );
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName
                                       )
                                   );
            auto iodaVarName = getIodaName(
                varName,
                IODA_VARIABLE_NAMES
            );
            auto var = group->getVar(
                iodaVarName
            );
            const std::vector<size_t> start = {0};
            const std::vector<size_t> count = {
                var.getDim(
                    0
                ).getSize()
            };
            var.getVar(
                start,
                count,
                *data
            );
            return 0;
        } catch (netCDF::exceptions::NcException &e) {
            return netcdfErrorMessage(
                e,
                __LINE__,
                __FILE__
            );
        }
    }

    int netcdfGetVarInt(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int *data
    ) {
        return netcdfGetVar(
            netcdfID,
            groupName,
            varName,
            &data
        );
    }

    int netcdfGetVarInt64(
        int netcdfID,
        const char *groupName,
        const char *varName,
        long long *data
    ) {
        return netcdfGetVar(
            netcdfID,
            groupName,
            varName,
            &data
        );
    }

    int netcdfGetVarReal(
        int netcdfID,
        const char *groupName,
        const char *varName,
        float *data
    ) {
        return netcdfGetVar(
            netcdfID,
            groupName,
            varName,
            &data
        );
    }

    int netcdfGetVarString(
        int netcdfID,
        const char *groupName,
        const char *varName,
        char ***data
    ) {
        auto file = FileMap::getInstance().getFile(
            netcdfID
        );
        const auto group = !groupName
                               ? file
                               : std::make_shared<
                                   netCDF::NcGroup>(
                                   file->getGroup(
                                       groupName
                                   )
                               );
        auto iodaVarName = getIodaName(
            varName,
            IODA_VARIABLE_NAMES
        );
        auto var = group->getVar(
            iodaVarName
        );
        auto dims = var.getDims();
        size_t numStrings = dims[0].getSize();
        char **buffer = new char *[numStrings];
        int retval = netcdfGetVar(
            netcdfID,
            groupName,
            varName,
            &buffer
        );
        size_t longestStringLength{};
        for (auto i = 0; i < numStrings; i++) {
            longestStringLength = std::max(
                longestStringLength,
                strlen(
                    buffer[i]
                )
            );
        }
        for (auto i = 0; i < numStrings; i++) {
            (*data)[i] = new char[longestStringLength + 1];
        }
        for (auto i = 0; i < numStrings; i++) {
            std::string tmpStr = buffer[i];
            std::copy(
                tmpStr.c_str(),
                tmpStr.c_str() + tmpStr.size() + 1,
                (*data)[i]
            );
            (*data)[i][tmpStr.size()] = '\0';
        }
        var.freeString(
            numStrings,
            buffer
        );
        delete[] buffer;
        return retval;
    }

    void netcdfFreeString(
        int numStrings,
        char ***data
    ) {
        for (auto i = 0; i < numStrings; i++) {
            delete [] (*data)[i];
            (*data)[i] = NULL;
        }
    }
}
