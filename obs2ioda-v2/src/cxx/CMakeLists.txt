set(obs2ioda_cxx_SOURCES
    netcdf_error.cc
    netcdf_file.cc
    netcdf_group.cc
    netcdf_dimension.cc
    netcdf_variable.cc
    netcdf_attribute.cc
)
set(obs2ioda_cxx_LIBRARIES
    NetCDF::NetCDF_CXX
    NetCDF::NetCDF_C
)
set(obs2ioda_cxx_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_library(obs2ioda_cxx SHARED ${obs2ioda_cxx_SOURCES})
obs2ioda_cxx_library(obs2ioda_cxx "${obs2ioda_cxx_INCLUDE_DIRS}" "${obs2ioda_cxx_LIBRARIES}")
