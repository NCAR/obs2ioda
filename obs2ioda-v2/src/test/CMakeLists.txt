set(test_utils_SOURCES
    test_utils.f90
)
list(TRANSFORM test_utils_SOURCES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
add_library(test_utils SHARED ${test_utils_SOURCES})
obs2ioda_fortran_library(test_utils "")