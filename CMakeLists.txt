# Check CMake version
cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include("${CMAKE_SOURCE_DIR}/cmake/Obs2Ioda_Functions.cmake")

include("${CMAKE_SOURCE_DIR}/cmake/create_testing_env.cmake")
enable_testing()
include(FetchContent)
# Define the project
project(obs2ioda LANGUAGES Fortran CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Fortran module output directory for build interface
set(OBS2IODA_MODULE_DIR ${PROJECT_NAME}/module/${CMAKE_Fortran_COMPILER_ID}/${CMAKE_Fortran_COMPILER_VERSION})
# Install Fortran module directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/${OBS2IODA_MODULE_DIR}/ DESTINATION ${CMAKE_INSTALL_LIBDIR}/${OBS2IODA_MODULE_DIR}/)

# Set the Fortran compiler and flags
option(CREATE_TESTING_ENV ON "Create a Python virtual environment for running test suite." )
if(CREATE_TESTING_ENV)
    include("${CMAKE_SOURCE_DIR}/cmake/create_testing_env.cmake")
endif()

# Set the Fortran compiler and flags
set(NCEP_BUFR_LIB CACHE STRING "" )

# Find required packages
find_package(NetCDF REQUIRED COMPONENTS Fortran C CXX)

add_subdirectory("${CMAKE_SOURCE_DIR}/obs2ioda-v3")
add_subdirectory("${CMAKE_SOURCE_DIR}/config")
add_subdirectory("${CMAKE_SOURCE_DIR}/test")
