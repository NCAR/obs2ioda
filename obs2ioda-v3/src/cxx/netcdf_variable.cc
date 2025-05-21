#include "netcdf_variable.h"
#include "netcdf_file.h"
#include "netcdf_error.h"
#include <algorithm>
#include <cstring>
#include <ioda_group.h>
#include <ioda_variable.h>

namespace Obs2Ioda {
    std::vector<char> flattenCharPtrArray(const char *const *values,
                                          const int numStrings,
                                          const int stringSize) {
        std::vector<char> contiguousValues(
            numStrings * stringSize + numStrings, ' ');

        for (int i = 0; i < numStrings; ++i) {
            int len = std::strlen(values[i]);
            std::copy_n(values[i], std::min(len, stringSize),
                        contiguousValues.begin() + i * stringSize);
            contiguousValues[i * stringSize + stringSize] = '\0';
        }
        return contiguousValues;
    }

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
            auto iodaVariable = IodaVariable(varName);
            if (iodaVariable.isV1Variable()) {
                auto iodaGroup = IodaGroup(varName);
                netCDF::NcGroup v1Group;
                if (file->getGroup(iodaGroup.getName()).isNull()) {
                    v1Group =
                            file->addGroup(iodaGroup.getName());
                } else {
                    v1Group = file->getGroup(iodaGroup.getName());
                }
                groupName = v1Group.getName().c_str();
                if (iodaVariable.isChannelVariable()) {
                    if (!file->getGroup(groupName).getVar(
                        iodaVariable.getName()).isNull()) {
                        return 0;
                    }
                }
            }
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           iodaSchema.getGroup(
                                               groupName)->
                                           getValidName()));
            std::vector<netCDF::NcDim> dims;
            dims.reserve(numDims);
            for (int i = 0; i < numDims; i++) {
                dims.push_back(file->getDim(
                    iodaSchema.getDimension(dimNames[i])->
                    getValidName()));;
            }
            if (iodaVariable.isChannelVariable()) {
                dims.push_back(file->getDim("Channel"));
            }
            auto iodaVarName = iodaVariable.getName();
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
            auto iodaVariable = IodaVariable(varName);
            if (iodaVariable.isV1Variable()) {
                auto iodaGroup = IodaGroup(varName);
                groupName = iodaGroup.getName().c_str();
            }
            const auto group = !groupName
                                   ? file
                                   : std::make_shared<
                                       netCDF::NcGroup>(
                                       file->getGroup(
                                           iodaSchema.getGroup(
                                               groupName)->
                                           getValidName()));
            auto iodaVarName = iodaVariable.getName();
            const auto var = group->getVar(iodaVarName);
            auto varType = var.getType();
            // Special handling for char arrays
            if (varType == netCDF::ncChar) {
                const auto contiguousValues = flattenCharPtrArray(
                    reinterpret_cast<const char * const *>(values),
                    static_cast<int>(var.getDims()[0].getSize()),
                    static_cast<int>(var.getDims()[1].getSize())
                );
                var.putVar(contiguousValues.data());
                return 0;
            }
            std::vector<size_t> start;
            std::vector<size_t> count;
            if (iodaVariable.isChannelVariable()) {
                start = {0, 0};
                count = {var.getDim(0).getSize(), 1};
                start[1] = iodaVariable.getChannelIndex(varName);
                var.putVar(start, count, values);
                return 0;
            }
            for (auto dim: var.getDims()) {
                start.push_back(0);
                count.push_back(dim.getSize());
                var.putVar(start, count, values);
                return 0;
            }
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

    int netcdfPutVarDouble(
        int netcdfID,
        const char *groupName,
        const char *varName,
        const double *values
    ) {
        return netcdfPutVar(
            netcdfID,
            groupName,
            varName,
            values
        );
    }

    int netcdfPutVarChar(
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
                                           iodaSchema.getGroup(
                                               groupName)->
                                           getValidName()));
            auto iodaVarName = iodaSchema.getVariable(varName)->
                    getValidName();
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
}
