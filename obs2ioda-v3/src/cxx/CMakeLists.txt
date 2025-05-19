FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

set(obs2ioda_cxx_SOURCES
    netcdf_error.cc
    netcdf_file.cc
    netcdf_group.cc
    netcdf_dimension.cc
    netcdf_variable.cc
    netcdf_attribute.cc
    ioda_obs_schema/ioda_obs_schema.cc
    ioda_obs_schema_map/ioda_obs_schema_map.cc
    ioda_attribute/ioda_attribute.cc
    ioda_dimension/ioda_dimension.cc
    ioda_group/ioda_group.cc
    ioda_variable/ioda_variable.cc
)
set(obs2ioda_cxx_LIBRARIES
    NetCDF::NetCDF_CXX
    NetCDF::NetCDF_C
    yaml-cpp::yaml-cpp
)
set(obs2ioda_cxx_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/generated
)
add_library(obs2ioda_cxx SHARED ${obs2ioda_cxx_SOURCES})
target_compile_definitions(obs2ioda_cxx PUBLIC OBS2IODA_ROOT_DIR="${CMAKE_SOURCE_DIR}")
obs2ioda_cxx_library(obs2ioda_cxx "${obs2ioda_cxx_INCLUDE_DIRS}" "${obs2ioda_cxx_LIBRARIES}")
add_subdirectory("ioda_obs_schema_map")
add_subdirectory("ioda_obs_schema")
add_subdirectory("ioda_group")
add_subdirectory("ioda_dimension")
add_subdirectory("ioda_variable")
add_subdirectory("ioda_attribute")
