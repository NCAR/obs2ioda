add_pfunit_ctest(Test_foo
                 TEST_SOURCES Test_foo_mod.pf
)
target_include_directories(Test_foo PRIVATE ${PFUNIT_INCLUDE_DIRS})