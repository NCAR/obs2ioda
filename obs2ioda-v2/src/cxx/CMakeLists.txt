set(obs2ioda_cxx_SOURCES
    netcdf_error.cc
)
set(obs2ioda_cxx_LIBRARIES
    NetCDF::NetCDF_CXX
    NetCDF::NetCDF_C
)
add_library(obs2ioda_cxx SHARED ${obs2ioda_cxx_SOURCES})
obs2ioda_cxx_library(obs2ioda_cxx "${obs2ioda_cxx_LIBRARIES}")
