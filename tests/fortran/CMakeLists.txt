# Specify Fortran standard
enable_language(Fortran)

# Include pFUnit's Fortran compiler wrapper
include(FortranCInterface)
FortranCInterface_VERIFY()
include_directories(${PFUNIT_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/build)

add_pfunit_ctest(Test_handle_user_input_mod
                 TEST_SOURCES Test_handle_user_input_mod.pf
                 LINK_LIBRARIES v2
)