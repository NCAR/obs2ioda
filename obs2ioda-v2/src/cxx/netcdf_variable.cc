#include "netcdf_variable.h"
#include "netcdf_file.h"
#include "netcdf_error.h"
#include <algorithm>
#include <mutex>

namespace Obs2Ioda {
    template int netcdfGetVar<int>(
        int,
        const char *,
        const char *,
        int **
    );

    template int netcdfGetVar<long long>(
        int,
        const char *,
        const char *,
        long long **
    );

    template int netcdfGetVar<float>(
        int,
        const char *,
        const char *,
        float **
    );

    template int netcdfSetFill<int>(
        int,
        const char *,
        const char *,
        int,
        int);

    template int netcdfSetFill<float>(
        int,
        const char *,
        const char *,
        int,
        float);

    template int netcdfSetFill<long long>(
        int,
        const char *,
        const char *,
        int,
        long long);

    template int netcdfSetFill<const char *>(
        int,
        const char *,
        const char *,
        int,
        const char *
    );

    template int netcdfSetFill<const char>(
        int,
        const char *,
        const char *,
        int,
        const char);

    template int netcdfPutVar<int>(
        int,
        const char *,
        const char *,
        const int *
    );

    template int netcdfPutVar<float>(
        int,
        const char *,
        const char *,
        const float *
    );

    int netcdfAddVar(
        int netcdfID,
        const char *groupName,
        const char *varName,
        nc_type netcdfDataType,
        int numDims,
        const char **dimNames
    ) {
        try {
            const auto file = FileMap::getInstance().getFile(netcdfID);
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName));
            std::vector<netCDF::NcDim> dims;
            dims.reserve(numDims);
            for (int i = 0; i < numDims; i++) {
                dims.push_back(file->getDim(dimNames[i]));;
            }
            auto var = group->addVar(
                varName,
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
        const T *data
    ) {
        try {
            const auto file = FileMap::getInstance().getFile(netcdfID);
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName));
            auto var = group->getVar(varName);
            var.putVar(data);
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
        const int *data
    ) {
        return netcdfPutVar(
            netcdfID,
            groupName,
            varName,
            data
        );
    }

    int netcdfPutVarInt64(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const long long *data
    ) {
        return netcdfPutVar(
            netcdfID,
            groupName,
            varName,
            data
        );
    }

    int netcdfPutVarReal(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const float *data
    ) {
        return netcdfPutVar(
            netcdfID,
            groupName,
            varName,
            data
        );
    }

    int netcdfPutVarString(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const char **data
    ) {
        return netcdfPutVar(
            netcdfID,
            groupName,
            varName,
            data
        );
    }


    template<typename T>
    int netcdfGetVar(
        int netcdfID,
        const char *groupName,
        const char *varName,
        T **data
    ) {
        try {
            const auto file = FileMap::getInstance().getFile(netcdfID);
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName));
            auto var = group->getVar(varName);
            const std::vector<size_t> start = {0};
            const std::vector<size_t> count = {var.getDim(0).getSize()};
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

    template<typename T>
    int netcdfSetFill(
        int netcdfID,
        const char *groupName,
        const char *varName,
        int fillMode,
        T fillValue
    ) {
        try {
            const auto file = FileMap::getInstance().getFile(netcdfID);
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           groupName));
            auto var = group->getVar(varName);
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

    int netcdfGetVarString1D(
        int netcdfID,
        const char *groupName,
        const char *varName,
        char ***data
    ) {
        auto file = FileMap::getInstance().getFile(netcdfID);
        auto dims = file->getVar(varName).getDims();
        auto var = file->getVar(varName);
        size_t numStrings = dims[0].getSize();
        char **buffer = new char *[numStrings];
        int retval = netcdfGetVar(
            netcdfID,
            groupName,
            varName,
            &buffer
        );
        for (auto i = 0; i < numStrings; i++) {
            std::string tmpStr = buffer[i];
            std::copy(
                tmpStr.begin(),
                tmpStr.end(),
                (*data)[i]
            );
            (*data)[i][tmpStr.size()] = '\0';
        }
        var.freeString(
            numStrings,
            (char **) buffer
        );
        free(buffer);
        return retval;
    }
}
