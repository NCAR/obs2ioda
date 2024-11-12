set(FORTRAN_COMMON_FLAGS
    $<$<COMPILE_LANGUAGE:Fortran>:-mcmodel=medium>
)

set(FORTRAN_GNU_FLAGS
    $<$<COMPILE_LANGUAGE:Fortran>:-cpp -ffree-line-length-none>
)
set(FORTRAN_GNU_DEBUG_FLAGS
    $<$<COMPILE_LANGUAGE:Fortran>:-fbacktrace -ffpe-trap=invalid,zero,overflow -fcheck=all>
)

set(FORTRAN_INTEL_FLAGS
    $<$<COMPILE_LANGUAGE:Fortran>:-fpp>
)

set(FORTRAN_INTEL_DEBUG_FLAGS
    $<$<COMPILE_LANGUAGE:Fortran>:-check uninit -ftrapuv -g -traceback -fpe0>
)
