# Check CMake version
cmake_minimum_required(VERSION 3.10)

enable_testing()

# Define the project
project(obs2ioda LANGUAGES Fortran)

include(GNUInstallDirs)
include("${CMAKE_SOURCE_DIR}/cmake/Obs2Ioda_Functions.cmake")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Fortran module output directory for build interface
set(OBS2IODA_MODULE_DIR ${PROJECT_NAME}/module/${CMAKE_Fortran_COMPILER_ID}/${CMAKE_Fortran_COMPILER_VERSION})
# Set the Fortran compiler and flags
set(NCEP_BUFR_LIB CACHE STRING "" )
# Set obs2ioda installation directory. If not set, default to the build directory
set(OBS2IODA_INSTALL_DIR CACHE STRING "" )
if (OBS2IODA_INSTALL_DIR STREQUAL "")
    set(OBS2IODA_INSTALL_DIR ${CMAKE_BINARY_DIR})
    message(STATUS "OBS2IODA_INSTALL_DIR not set. Defaulting to ${OBS2IODA_INSTALL_DIR}")
endif ()
set(CMAKE_INSTALL_PREFIX  ${OBS2IODA_INSTALL_DIR})
# Install Fortran module directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/${OBS2IODA_MODULE_DIR}/ DESTINATION ${CMAKE_INSTALL_LIBDIR}/${OBS2IODA_MODULE_DIR}/)

# Find required packages
find_package(NetCDF REQUIRED COMPONENTS Fortran C)

add_subdirectory("${CMAKE_SOURCE_DIR}/obs2ioda-v2")
add_subdirectory("${CMAKE_SOURCE_DIR}/test")
