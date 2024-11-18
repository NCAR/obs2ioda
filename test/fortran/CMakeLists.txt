add_pfunit_ctest(Test_f_c_string_t_mod
                 TEST_SOURCES Test_f_c_string_t_mod.pf
                 LINK_LIBRARIES v2
)
target_include_directories(Test_f_c_string_t_mod PRIVATE ${PFUNIT_INCLUDE_DIRS})
add_memcheck_ctest(Test_f_c_string_t_mod)

add_pfunit_ctest(Test_f_c_string_1D_t_mod
                 TEST_SOURCES Test_f_c_string_1D_t_mod.pf
                 LINK_LIBRARIES v2
)
target_include_directories(Test_f_c_string_1D_t_mod PRIVATE ${PFUNIT_INCLUDE_DIRS})
add_memcheck_ctest(Test_f_c_string_1D_t_mod)

add_pfunit_ctest(Test_f_c_real_1D_t_mod
                 TEST_SOURCES Test_f_c_real_1D_t_mod.pf
                 LINK_LIBRARIES v2
)
target_include_directories(Test_f_c_real_1D_t_mod PRIVATE ${PFUNIT_INCLUDE_DIRS})
add_memcheck_ctest(Test_f_c_real_1D_t_mod)

add_pfunit_ctest(Test_f_c_real_2D_t_mod
                 TEST_SOURCES Test_f_c_real_2D_t_mod.pf
                 LINK_LIBRARIES v2
)
target_include_directories(Test_f_c_real_2D_t_mod PRIVATE ${PFUNIT_INCLUDE_DIRS})
add_memcheck_ctest(Test_f_c_real_2D_t_mod)

add_pfunit_ctest(Test_f_c_int_1D_t_mod
                 TEST_SOURCES Test_f_c_int_1D_t_mod.pf
                 LINK_LIBRARIES v2
)
target_include_directories(Test_f_c_int_1D_t_mod PRIVATE ${PFUNIT_INCLUDE_DIRS})
add_memcheck_ctest(Test_f_c_int_1D_t_mod)

add_pfunit_ctest(Test_f_c_double_1D_t_mod
                 TEST_SOURCES Test_f_c_double_1D_t_mod.pf
                 LINK_LIBRARIES v2
)
target_include_directories(Test_f_c_double_1D_t_mod PRIVATE ${PFUNIT_INCLUDE_DIRS})
add_memcheck_ctest(Test_f_c_double_1D_t_mod)