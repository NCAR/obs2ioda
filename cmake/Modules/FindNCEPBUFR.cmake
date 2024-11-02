# FindNCEPBUFR.cmake
# This script finds the first occurrence of a library matching "*libbufr*" with extensions
# .a, .so, or .dylib in NCEPBUFR_ROOT and creates an imported target.

# Ensure NCEPBUFR_ROOT is defined
if(NOT DEFINED NCEPBUFR_ROOT)
    message(WARNING "NCEPBUFR_ROOT is not defined. Please set it to the root directory of the NCEP BUFR installation.")
    set(NCEPBUFR_FOUND FALSE)
    return()
endif()

# Recursive search for any file containing "libbufr" in its name and ending with .a, .so, or .dylib
file(GLOB_RECURSE NCEPBUFR_LIB_FILES
     "${NCEPBUFR_ROOT}/*libbufr*.a"
     "${NCEPBUFR_ROOT}/*libbufr*.so"
     "${NCEPBUFR_ROOT}/*libbufr*.dylib"
)

# Check if any matching file was found
if(NCEPBUFR_LIB_FILES)
    list(GET NCEPBUFR_LIB_FILES 0 NCEPBUFR_LIB_FILE)  # Take the first match
    set(NCEPBUFR_FOUND TRUE)
    message(STATUS "Found libbufr library: ${NCEPBUFR_LIB_FILE}")

    # Create an imported target for the found library
    add_library(NCEPBUFR::libbufr UNKNOWN IMPORTED)
    set_target_properties(NCEPBUFR::libbufr PROPERTIES
                          IMPORTED_LOCATION "${NCEPBUFR_LIB_FILE}"
    )

else()
    set(NCEPBUFR_FOUND FALSE)
    message(WARNING "Could not find libbufr library in ${NCEPBUFR_ROOT}")
endif()
