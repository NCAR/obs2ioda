
set(netcdf_mod_SOURCES
        netcdf_mod.f90
)
set(goes_abi_converter_SOURCES
        goes_abi_converter.f90
)
list(TRANSFORM netcdf_mod_SOURCES PREPEND "src/")
list(TRANSFORM goes_abi_converter_SOURCES PREPEND "src/")
set(netcdf_mod_PUBLIC_LINK_LIBRARIES obs2ioda_cxx)
add_library(netcdf_mod SHARED ${netcdf_mod_SOURCES})
obs2ioda_fortran_library(netcdf_mod "${netcdf_mod_PUBLIC_LINK_LIBRARIES}")
set(goes_abi_converter_PUBLIC_LINK_LIBRARIES netcdf_mod v3)
add_executable(goes_abi_converter ${goes_abi_converter_SOURCES})
obs2ioda_fortran_executable(goes_abi_converter "${goes_abi_converter_PUBLIC_LINK_LIBRARIES}")
