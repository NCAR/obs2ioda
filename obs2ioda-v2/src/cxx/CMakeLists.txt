set(obs2ioda_cxx_SOURCES
    netcdf_error.cc
    netcdf_file.cc
    netcdf_group.cc
    netcdf_dimension.cc
    netcdf_variable.cc
    netcdf_attribute.cc
    ioda_schema.cc
)
set(obs2ioda_cxx_LIBRARIES
    NetCDF::NetCDF_CXX
    NetCDF::NetCDF_C
    yaml-cpp::yaml-cpp
)
set(obs2ioda_cxx_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_library(obs2ioda_cxx SHARED ${obs2ioda_cxx_SOURCES})
target_compile_definitions(obs2ioda_cxx PUBLIC -DIODA_SCHEMA_YAML="${CMAKE_SOURCE_DIR}/share/ObsSpace.yaml")
obs2ioda_cxx_library(obs2ioda_cxx "${obs2ioda_cxx_INCLUDE_DIRS}" "${obs2ioda_cxx_LIBRARIES}")
