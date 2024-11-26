
# Set include directories
include_directories(${NetCDF_INCLUDE_DIRS} )

set(netcdf_c_SOURCES
    netcdf_attribute.cc
    netcdf_dimension.cc
    netcdf_group.cc
    netcdf_variable.cc
    netcdf_file.cc
    netcdf_utils.cc
    ioda_names.cc
)
list(TRANSFORM netcdf_c_SOURCES PREPEND "src/cxx/")

# Define sources
set(v2_SOURCES
    define_mod.f90
    gnssro_mod.f90
    hsd.f90
    satwnd_mod.f90
    kinds.f90
    main.f90
    ncio_mod.f90
    netcdf_mod.f90
    prepbufr_mod.f90
    radiance_mod.f90
    ufo_variables_mod.F90
    utils_mod.f90
    f_c_string_t_mod.f90
    f_c_string_1D_t_mod.f90
    f_c_numeric_mod.f90
    netcdf_c_i_mod.f90
)
list(TRANSFORM v2_SOURCES PREPEND "src/fortran/")
set(v2_MAIN_SOURCE
    main.f90
)
list(TRANSFORM v2_MAIN_SOURCE PREPEND "src/fortran/")
add_library(netcdf_c SHARED ${netcdf_c_SOURCES})
set_target_properties(netcdf_c PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(netcdf_c PUBLIC ${NetCDF_INCLUDE_DIRS})
target_link_libraries(netcdf_c PUBLIC  NetCDF::NetCDF_CXX NetCDF::NetCDF_C)
set(v2_PUBLIC_LINK_LIBRARIES "${NCEP_BUFR_LIB}")
add_library(v2 SHARED ${v2_SOURCES})
target_include_directories(v2 PUBLIC ${NetCDF_INCLUDE_DIRS})
obs2ioda_fortran_target(v2 ${v2_MAIN_SOURCE})

