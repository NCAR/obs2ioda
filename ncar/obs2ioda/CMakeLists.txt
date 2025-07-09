# Check CMake version
cmake_minimum_required(VERSION 3.10)

enable_testing()
include(FetchContent)
# Define the project
project(obs2ioda LANGUAGES Fortran CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Fortran module output directory for build interface
set(OBS2IODA_MODULE_DIR ${PROJECT_NAME}/module/${CMAKE_Fortran_COMPILER_ID}/${CMAKE_Fortran_COMPILER_VERSION})
# Install Fortran module directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/${OBS2IODA_MODULE_DIR}/ DESTINATION ${CMAKE_INSTALL_LIBDIR}/${OBS2IODA_MODULE_DIR}/)


find_package(NetCDF REQUIRED COMPONENTS CXX C)

add_subdirectory("${CMAKE_SOURCE_DIR}/config")
add_subdirectory("${CMAKE_SOURCE_DIR}/src")
