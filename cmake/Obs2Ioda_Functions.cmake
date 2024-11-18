include("${CMAKE_SOURCE_DIR}/cmake/Obs2Ioda_CompilerFlags.cmake")

<<<<<<< HEAD
# This CMake function, `obs2ioda_fortran_library_target`, configures Fortran library targets for obs2ioda.
#
# Its arguments are:
# - target: the name of the target to configure
# - public_link_libraries: the public link libraries associated with the target
=======
# This CMake function, `obs2ioda_fortran_target`, configures Fortran targets for obs2ioda.
#
# Its arguments are:
# - target: the name of the target to configure
# - target_main: the main source file for the executable
>>>>>>> 4177b09 (Introduce Fortran Test Framework and CTest Integration)
#
# The function sets the following properties for the target:
# - The directory for Fortran module files
# - The include directories for the target (both build and install interfaces)
# - The install RPATH to enable finding shared libraries at runtime
# - Fortran format as FREE
# - Compiler-specific options and flags, depending on whether the GNU Fortran or Intel Fortran compiler is used,
#   and whether the build type is Debug or not
#
<<<<<<< HEAD
# The function also links the provided public libraries to the target.
function(obs2ioda_fortran_library target public_link_libraries)
=======
# The function also links the public libraries to the target and creates an executable `obs2ioda_${target}` linked
# to the original target.
function(obs2ioda_fortran_target target target_main)
>>>>>>> 4177b09 (Introduce Fortran Test Framework and CTest Integration)
    set_target_properties(${target} PROPERTIES Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/${OBS2IODA_MODULE_DIR})
    target_include_directories(${target} INTERFACE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/${OBS2IODA_MODULE_DIR}>
                               $<INSTALL_INTERFACE:${OBS2IODA_MODULE_DIR}>)
    #Relocatable, portable, runtime dynamic linking
    set_target_properties(${target} PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
    # Global Fortran configuration
<<<<<<< HEAD
=======
    set(public_link_libraries_name ${target}_PUBLIC_LINK_LIBRARIES)
    set(public_link_libraries ${${public_link_libraries_name}})
>>>>>>> 4177b09 (Introduce Fortran Test Framework and CTest Integration)
    set_target_properties(${target} PROPERTIES Fortran_FORMAT FREE)

    # Compiler-specific options and flags
    set(OBS2IODA_FORTRAN_TARGET_COMPILE_OPTIONS_PRIVATE "")
    if (CMAKE_Fortran_COMPILER_ID MATCHES GNU)
        list(APPEND OBS2IODA_FORTRAN_TARGET_COMPILE_OPTIONS_PRIVATE
             ${FORTRAN_COMPILER_GNU_FLAGS}
        )
        if (CMAKE_BUILD_TYPE MATCHES Debug)
            list(APPEND OBS2IODA_FORTRAN_TARGET_COMPILE_OPTIONS_PRIVATE
                 ${FORTRAN_COMPILER_GNU_DEBUG_FLAGS}
            )
        endif ()
    elseif (CMAKE_Fortran_COMPILER_ID MATCHES Intel)
        list(APPEND OBS2IODA_FORTRAN_TARGET_COMPILE_OPTIONS_PRIVATE
             ${FORTRAN_COMPILER_INTEL_FLAGS}
        )
        if (CMAKE_BUILD_TYPE MATCHES Debug)
            list(APPEND OBS2IODA_FORTRAN_TARGET_COMPILE_OPTIONS_PRIVATE
                 ${FORTRAN_COMPILER_INTEL_DEBUG_FLAGS}
            )
        endif ()
    endif ()
    target_compile_options(${target} PRIVATE ${OBS2IODA_FORTRAN_TARGET_COMPILE_OPTIONS_PRIVATE})
    target_link_libraries(${target} PUBLIC ${public_link_libraries})
<<<<<<< HEAD
endfunction()

# This CMake function, `obs2ioda_fortran_executable`, configures the installation and linking of Fortran executables for obs2ioda.
#
# Its arguments are:
# - target: the name of the executable target to configure
# - public_link_libraries: the public link libraries associated with the target
#
# The function performs the following:
# - Sets the install RPATH for the target to enable finding shared libraries relative to the executable's location.
# - Links the provided public libraries to the target using `target_link_libraries`.
#
# This ensures that the target has the correct runtime library paths and is properly linked with its public dependencies.
function(obs2ioda_fortran_executable target public_link_libraries)
    set_target_properties(${target} PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
    target_link_libraries(${target} PUBLIC ${public_link_libraries})
endfunction()

# This CMake function, `obs2ioda_cxx_library`, configures C++ targets for obs2ioda.
#
# Its arguments are:
# - target: the name of the C++ target to configure
# - public_link_libraries: the public link libraries associated with the target
#
# The function performs the following:
# * Sets the `INSTALL_RPATH` property for the target, ensuring that shared libraries can be found
#    relative to the target's installation directory.
# * Links the provided public libraries to the target using `target_link_libraries`.
#
# This setup ensures that the target is correctly linked with its public dependencies and that
# runtime shared library paths are properly configured for relocatable installations.
function(obs2ioda_cxx_library target public_link_libraries)
    set_target_properties(${target} PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}")
    target_link_libraries(${target} PUBLIC ${public_link_libraries})
=======
    add_executable(obs2ioda_${target} ${target_main})
    target_link_libraries(obs2ioda_${target} PUBLIC ${target})

endfunction()

# Function: add_memcheck_ctest
# Adds a memory check test for a given target using Valgrind.
#
# Arguments:
# - target (string): The name of the target to check for memory issues.
#
# Behavior:
# - If Valgrind is found, it adds a CTest named `<target>_memcheck` that runs the target
#   with Valgrind's memory checking options (`--leak-check=full`, `--error-exitcode=1`).
# - If Valgrind is not found, it outputs a status message and does not add the memory check.
#
# Example Usage:
# add_memcheck_ctest(my_target)
#
# Notes:
# - Ensure Valgrind is installed and accessible in the system's PATH for this function to work.
function(add_memcheck_ctest target)
    find_program(VALGRIND "valgrind")
    if (VALGRIND)
        message(STATUS "Valgrind found: ${VALGRIND}")
        message(STATUS "Adding memory check for test: ${target}")
        set(VALGRIND_COMMAND valgrind --leak-check=full --error-exitcode=1 --undef-value-errors=no)
        add_test(NAME ${target}_memcheck
                 COMMAND ${VALGRIND_COMMAND} $<TARGET_FILE:${target}> memcheck)
    else ()
        message(STATUS "Valgrind not found")
        message(STATUS "Memory check for test: ${target} will not be added")
    endif ()
endfunction()

# Function: add_fortran_ctest
# Creates and registers a CTest for a Fortran test executable, handling mixed Fortran and C sources.
#
# Arguments:
# - test_name (string): The name of the test.
# - test_sources (list): List of source files for the test, including Fortran and optional C sources.
# - library_deps (list): List of library dependencies to link with the test executable.
#
# Behavior:
# - Identifies C source files (`*.c`) from the `test_sources` list and compiles them into shared libraries.
# - Updates the `library_deps` list to include the created shared libraries for C sources.
# - Creates a Fortran executable target named `Test_<test_name>` using the remaining Fortran sources.
# - Links the test executable with the specified libraries and any generated C libraries.
# - Registers the test with CTest, with the executable's path resolved to `${CMAKE_BINARY_DIR}/bin`.
function(add_fortran_ctest test_name test_sources library_deps)
    foreach(test_source ${test_sources})
        if (${test_source} MATCHES ".*\\.c$")
            get_filename_component(test_source_name ${test_source} NAME_WE)
            message(STATUS "Adding C test: ${test_source_name}")
            add_library("c_${test_source_name}" SHARED ${test_source})
            list(APPEND library_deps "c_${test_source_name}")
            list(REMOVE_ITEM test_sources ${test_source})
        endif ()
    endforeach ()
    add_executable("Test_${test_name}" ${test_sources})
    target_link_libraries("Test_${test_name}" ${library_deps})
    add_test(NAME ${test_name}
             COMMAND ${CMAKE_BINARY_DIR}/bin/Test_${test_name})
endfunction()
