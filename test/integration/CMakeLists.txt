set(run_obs2ioda_SOURCES
    run_obs2ioda.cc
)
add_executable(run_obs2ioda ${run_obs2ioda_SOURCES})
target_compile_definitions(run_obs2ioda PUBLIC -DTEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(run_obs2ioda PUBLIC obs2ioda_cxx GTest::gmock_main)
add_test(
        NAME run_obs2ioda_test
        COMMAND run_obs2ioda --gtest_filter=*
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_memcheck_ctest(run_obs2ioda)

set(check_groups_SOURCES
    check_groups.cc
)
add_executable(check_groups ${check_groups_SOURCES})
target_link_libraries(check_groups PUBLIC obs2ioda_cxx GTest::gmock_main)
add_test(
        NAME check_groups_test
        COMMAND check_groups --gtest_filter=*
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(check_variables_SOURCES
        check_variables.cc
)
add_executable(check_variables ${check_variables_SOURCES})
target_link_libraries(check_variables PUBLIC obs2ioda_cxx GTest::gmock_main)
add_test(
        NAME check_variables_test
        COMMAND check_variables --gtest_filter=*
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(check_attributes_SOURCES
        check_attributes.cc
)
add_executable(check_attributes ${check_attributes_SOURCES})
target_link_libraries(check_attributes PUBLIC obs2ioda_cxx GTest::gmock_main)
add_test(
        NAME check_attributes_test
        COMMAND check_attributes --gtest_filter=*
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
