
add_pfunit_ctest( Test_set_brit_ahi_mod
                  TEST_SOURCES Test_set_brit_ahi_mod.pf
                  LINK_LIBRARIES v2
                  )
target_include_directories(Test_set_brit_ahi_mod PRIVATE ${PFUNIT_INCLUDE_DIRS})