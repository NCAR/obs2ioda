# Set Fortran compiler flags specific to the GNU Compiler
# -cpp: Enable the C preprocessor
# -ffree-line-length-none: Remove the limit on the length of lines in the source file
# -mcmodel=medium: Allow for larger datasets in memory
set(FORTRAN_COMPILER_GNU_FLAGS
    $<$<COMPILE_LANGUAGE:Fortran>:-std=2008 -mcmodel=medium>
)

# Set Debugging Fortran compiler flags specific to the GNU Compiler
# -fbacktrace: Provide a backtrace when an error occurs
# -ffpe-trap=invalid,zero,overflow: Trap floating point exceptions (invalid calculation, divide by zero, overflow)
# -fcheck=all: Execute all types of runtime checks
set(FORTRAN_COMPILER_GNU_DEBUG_FLAGS
    $<$<COMPILE_LANGUAGE:Fortran>:-g -fbacktrace -ffpe-trap=invalid,zero,overflow -fcheck=all>
)

# Set Fortran compiler flags for the Intel Compiler
# -fpp: Enable the preprocessing stage
set(FORTRAN_COMPILER_INTEL_FLAGS
    $<$<COMPILE_LANGUAGE:Fortran>:-stand f08 -mcmodel=medium>
)

# Set Debugging Fortran compiler flags for the Intel Compiler
# -check uninit: Checks uninitialized variables
# -ftrapuv: Enable trapping of uninitialized variables
# -g: Enable production of debug information
# -traceback: Give symbolic traceback on errors
# -fpe0: Stop execution when a floating-point exception occurs
set(FORTRAN_COMPILER_INTEL_DEBUG_FLAGS
    $<$<COMPILE_LANGUAGE:Fortran>:-check uninit -ftrapuv -g -traceback -fpe0>
)