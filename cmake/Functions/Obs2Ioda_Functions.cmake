function(obs2ioda_fortran_target target target_main)
    set_target_properties(${target} PROPERTIES Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/${OBS2IODA_MODULE_DIR})
    target_include_directories(${target} INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/${OBS2IODA_MODULE_DIR}>
                               $<INSTALL_INTERFACE:${OBS2IODA_MODULE_DIR}>)
    #Relocatable, portable, runtime dynamic linking
    set_target_properties(${target} PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
    # Global Fortran configuration
    set(public_link_libraries_name ${target}_PUBLIC_LINK_LIBRARIES)
    set(public_link_libraries ${${public_link_libraries_name}})
    set_target_properties(${target} PROPERTIES Fortran_FORMAT FREE)

    # Compiler-specific options and flags
    set(OBS2IODA_FORTRAN_TARGET_COMPILE_OPTIONS_PRIVATE
        $<$<COMPILE_LANGUAGE:Fortran>:-mcmodel=medium>
    )
    if (CMAKE_Fortran_COMPILER_ID MATCHES GNU)
        list(APPEND OBS2IODA_FORTRAN_TARGET_COMPILE_OPTIONS_PRIVATE
             $<$<COMPILE_LANGUAGE:Fortran>:-cpp -ffree-line-length-none>
        )
    elseif (CMAKE_Fortran_COMPILER_ID MATCHES Intel)
        list(APPEND OBS2IODA_FORTRAN_TARGET_COMPILE_OPTIONS_PRIVATE
             $<$<COMPILE_LANGUAGE:Fortran>:-fpp>
        )
    endif ()
    target_compile_options(${target} PRIVATE ${OBS2IODA_FORTRAN_TARGET_COMPILE_OPTIONS_PRIVATE})
    target_link_libraries(${target} PUBLIC ${public_link_libraries} netcdf_c NetCDF::NetCDF_Fortran NetCDF::NetCDF_CXX NetCDF::NetCDF_C)
    add_executable(obs2ioda_${target} ${target_main})
    target_link_libraries(obs2ioda_${target} PUBLIC ${target})
endfunction()

# Function: add_memcheck_ctest
#
# Description:
#   This function adds a memory check command as a test case in CTest
#   for the given target, using the Valgrind tool. If the Valgrind
#   tool is not found, it will print a message specifying that the
#   memory check could not be added for the given target.
#
#   The Valgrind command executed as part of the test includes options
#   for full leak checking (--leak-check=full), exiting with error status
#   if any leaks are detected (--error-exitcode=1), and keeping track of
#   the origin of uninitialized values (--track-origins=yes).
#
# Arguments:
#   target: The name of the target for which the memory check will be added.
#
# Usage:
#   add_memcheck_ctest(my_target)
#
# Example:
#   add_memcheck_ctest(my_executable)
#
function(add_memcheck_ctest target)
    find_program(VALGRIND "valgrind")
    if(VALGRIND)
        message(STATUS "Valgrind found: ${VALGRIND}")
        message(STATUS "Adding memory check for test: ${target}")
        set(VALGRIND_COMMAND valgrind --leak-check=full --error-exitcode=1 --undef-value-errors=no)
        add_test(NAME ${target}_memcheck
                 COMMAND ${VALGRIND_COMMAND} $<TARGET_FILE:${target}>)
    else()
        message(STATUS "Valgrind not found")
        message(STATUS "Memory check for test: ${target} will not be added")
    endif()
endfunction()