
FetchContent_Declare(
        pfunit
        GIT_REPOSITORY https://github.com/Goddard-Fortran-Ecosystem/pFUnit.git
        GIT_TAG v4.10.0
)

# Set pfunit CMake options **before** it's made available
set(SKIP_MPI ON CACHE BOOL "" FORCE)
set(SKIP_OPENMP ON CACHE BOOL "" FORCE)
set(SKIP_FHAMCREST ON CACHE BOOL "" FORCE)
set(ENABLE_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(pfunit)
