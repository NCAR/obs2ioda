set(write_netcdf_obs_test_SOURCES
        "write_netcdf_obs.test.f90"
)
set(write_netcdf_obs_test_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
set(write_netcdf_obs_test_LIBRARY_DEPENDENCIES
       netcdf_mod v3
)
add_executable(write_netcdf_obs_test
        "${write_netcdf_obs_test_SOURCES}"
)
target_include_directories(write_netcdf_obs_test
        PRIVATE
        "${write_netcdf_obs_test_INCLUDE_DIRECTORIES}"
)
target_link_libraries(write_netcdf_obs_test
        PRIVATE
        "${write_netcdf_obs_test_LIBRARY_DEPENDENCIES}"
)
add_test(
        NAME write_netcdf_obs_test
        COMMAND write_netcdf_obs_test
)