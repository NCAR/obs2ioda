# This CMake function, `obs2ioda_cxx_library`, configures C++ targets for obs2ioda.
#
# Its arguments are:
# - target: the name of the C++ target to configure
# - include_dirs: the include directories associated with the target
# - public_link_libraries: the public link libraries associated with the target
#
# The function performs the following:
# * Sets the `INSTALL_RPATH` property for the target, ensuring that shared libraries can be found
#    relative to the target's installation directory.
# * Links the provided public libraries to the target using `target_link_libraries`.
# * Sets the include directories for the target using `target_include_directories`.
#
# This setup ensures that the target is correctly linked with its public dependencies and that
# runtime shared library paths are properly configured for relocatable installations, and that the target
# can find its include directories.
function(obs2ioda_cxx_library target include_dirs public_link_libraries)
    set_target_properties(${target} PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
    target_link_libraries(${target} PUBLIC ${public_link_libraries})
    target_include_directories(${target} PUBLIC ${include_dirs})
endfunction()

add_subdirectory(test)
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
    ioda_obs_schema.cc
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
