#include "netcdf_variable.h"
#include "netcdf_file.h"
#include "netcdf_error.h"
#include "netcdf_get_variable.h"

namespace Obs2Ioda {
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
            auto var = group->getVar(
                varName
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
            auto var = group->getVar(
                varName
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
        int **data
    ) {
        return netcdfGetVar(
            netcdfID,
            groupName,
            varName,
            data
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
        auto var = group->getVar(
            varName
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
