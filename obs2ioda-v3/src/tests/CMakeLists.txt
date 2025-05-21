set(fortran_ioda_v1_to_v3_test_SOURCES
        "ioda_v1_to_v3.test.f90"
)
set(fortran_ioda_v1_to_v3_test_INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}"
)
set(fortran_ioda_v1_to_v3_test_LIBRARY_DEPENDENCIES
        v3
)
add_executable(fortran_ioda_v1_to_v3_test
        "${fortran_ioda_v1_to_v3_test_SOURCES}"
)
target_include_directories(fortran_ioda_v1_to_v3_test
        PRIVATE
        "${fortran_ioda_v1_to_v3_test_INCLUDE_DIRECTORIES}"
)
target_link_libraries(fortran_ioda_v1_to_v3_test
        PRIVATE
        "${fortran_ioda_v1_to_v3_test_LIBRARY_DEPENDENCIES}"
)
add_test(
        NAME fortran_ioda_v1_to_v3_test
        COMMAND fortran_ioda_v1_to_v3_test
)