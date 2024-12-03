
# Set include directories
include_directories(${NetCDF_INCLUDE_DIRS} )

set(netcdf_cxx_SOURCES
    netcdf_attribute.cc
    netcdf_dimension.cc
    netcdf_group.cc
    netcdf_variable.cc
    netcdf_file.cc
    netcdf_utils.cc
    ioda_names.cc
)
list(TRANSFORM netcdf_cxx_SOURCES PREPEND "src/cxx/")

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
    netcdf_cxx_i_mod.f90
    netcdf_cxx_mod.f90
)
list(TRANSFORM v2_SOURCES PREPEND "src/fortran/")
set(v2_MAIN_SOURCE
    main.f90
)
list(TRANSFORM v2_MAIN_SOURCE PREPEND "src/fortran/")
add_library(netcdf_cxx SHARED ${netcdf_cxx_SOURCES})
target_include_directories(netcdf_cxx PUBLIC ${NetCDF_INCLUDE_DIRS})
set(netcdf_cxx_PUBLIC_LINK_LIBRARIES NetCDF::NetCDF_CXX NetCDF::NetCDF_C)
obs2ioda_cxx_target(netcdf_cxx ${netcdf_cxx_SOURCES})
set(v2_PUBLIC_LINK_LIBRARIES "${NCEP_BUFR_LIB}" NetCDF::NetCDF_CXX NetCDF::NetCDF_C NetCDF::NetCDF_Fortran netcdf_cxx)
add_library(v2 SHARED ${v2_SOURCES})
target_include_directories(v2 PUBLIC ${NetCDF_INCLUDE_DIRS})
obs2ioda_fortran_target(v2 ${v2_MAIN_SOURCE})
