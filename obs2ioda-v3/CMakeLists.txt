
# Set include directories
include_directories(${NetCDF_INCLUDE_DIRS} )

# Define sources
set(v3_SOURCES
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
)
list(TRANSFORM v3_SOURCES PREPEND "src/")
set(v3_MAIN_SOURCE
    main.f90
)
list(TRANSFORM v3_MAIN_SOURCE PREPEND "src/")
set(v3_PUBLIC_LINK_LIBRARIES "${NetCDF_LIBRARIES}" "${NCEP_BUFR_LIB}")
add_library(v3 SHARED ${v3_SOURCES})
obs2ioda_fortran_target(v3 ${v3_MAIN_SOURCE})

