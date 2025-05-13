
# Define sources
set(netcdf_mod_SOURCES
        netcdf_mod.f90
)
set(goes_abi_SOURCES
        goes_abi_converter.f90
)
list(TRANSFORM netcdf_mod_SOURCES PREPEND "src/")
list(TRANSFORM goes_abi_SOURCES PREPEND "src/")
set(netcdf_mod_PUBLIC_LINK_LIBRARIES NetCDF::NetCDF_Fortran NetCDF::NetCDF_C "${NCEP_BUFR_LIB}")
add_library(netcdf_mod SHARED ${netcdf_mod_SOURCES})
obs2ioda_fortran_library(netcdf_mod "${netcdf_mod_PUBLIC_LINK_LIBRARIES}")
set(goes_abi_PUBLIC_LINK_LIBRARIES netcdf_mod)
add_executable(goes_abi ${goes_abi_SOURCES})
obs2ioda_fortran_executable(goes_abi "${goes_abi_PUBLIC_LINK_LIBRARIES}")

