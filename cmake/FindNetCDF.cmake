set(NetCDF_FOUND FALSE)

# Include names for each component
set(NetCDF_C_INCLUDE_NAME netcdf.h)
set(NetCDF_CXX_INCLUDE_NAME netcdf)
set(NetCDF_Fortran_INCLUDE_NAME netcdf.mod)

# Library names for each component
set(NetCDF_C_LIBRARY_NAME netcdf)
set(NetCDF_CXX_LIBRARY_NAME netcdf_c++4)
set(NetCDF_Fortran_LIBRARY_NAME netcdff)

list(APPEND _possible_components C CXX Fortran)

foreach(_comp IN LISTS _possible_components)
  if(_comp STREQUAL "C")
    set(_config_executable nc-config)
  elseif(_comp STREQUAL "CXX")
    set(_config_executable ncxx4-config)
  elseif(_comp STREQUAL "Fortran")
    set(_config_executable nf-config)
  endif()

  find_program(NetCDF_${_comp}_CONFIG_EXECUTABLE ${_config_executable}
          HINTS ${NetCDF_INCLUDE_DIRS}
          PATH_SUFFIXES bin
          DOC "NetCDF ${_config_executable} helper")

  if(NetCDF_${_comp}_CONFIG_EXECUTABLE)
    set(NetCDF_${_comp}_FOUND TRUE)

    if (${_comp} STREQUAL "Fortran")
      execute_process(COMMAND ${NetCDF_${_comp}_CONFIG_EXECUTABLE} --flibs
              OUTPUT_VARIABLE NetCDF_${_comp}_LIBRARIES
              OUTPUT_STRIP_TRAILING_WHITESPACE)
    else()
      execute_process(COMMAND ${NetCDF_${_comp}_CONFIG_EXECUTABLE} --libs
              OUTPUT_VARIABLE NetCDF_${_comp}_LIBRARIES
              OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()


    # Get include flags
    execute_process(COMMAND ${NetCDF_${_comp}_CONFIG_EXECUTABLE} --includedir
            OUTPUT_VARIABLE NetCDF_${_comp}_INCLUDE_DIR
            OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Define imported targets
    add_library(NetCDF::NetCDF_${_comp} INTERFACE IMPORTED)
    set_target_properties(NetCDF::NetCDF_${_comp} PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${NetCDF_${_comp}_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "${NetCDF_${_comp}_LIBRARIES}"
    )
  endif()
endforeach()

# Finalize found status
if(NetCDF_C_FOUND OR NetCDF_CXX_FOUND OR NetCDF_Fortran_FOUND)
  set(NetCDF_FOUND TRUE)
endif()