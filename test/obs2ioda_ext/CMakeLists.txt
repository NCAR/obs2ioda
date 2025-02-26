set(obs2ioda_cxx_ext_SOURCES
    netcdf_get_variable.cc
)
list(TRANSFORM obs2ioda_cxx_ext_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(obs2ioda_cxx_ext_LIBRARIES
        NetCDF::NetCDF_CXX
        NetCDF::NetCDF_C
        obs2ioda_cxx
)
set(obs2ioda_cxx_ext_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
)
add_library(obs2ioda_cxx_ext SHARED ${obs2ioda_cxx_ext_SOURCES})
obs2ioda_cxx_library(obs2ioda_cxx_ext "${obs2ioda_cxx_ext_INCLUDE_DIRS}" "${obs2ioda_cxx_ext_LIBRARIES}")

set(v2_ext_SOURCES
        netcdf_cxx_get_variable_mod.f90
        netcdf_cxx_get_variable_i_mod.f90
)
list(TRANSFORM v2_ext_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(v2_ext_LIBRARIES
        v2 obs2ioda_cxx obs2ioda_cxx_ext
)
set(v2_ext_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
)
add_library(v2_ext SHARED ${v2_ext_SOURCES})
obs2ioda_fortran_library(v2_ext "${v2_ext_LIBRARIES}")
