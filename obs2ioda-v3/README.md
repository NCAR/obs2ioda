# obs2ioda-v3.x

## Installation

`obs2ioda-v3` uses CMake as its build system. Follow these steps to build the project:

### Prerequisites

Ensure the following libraries are installed:
- NetCDF
- NCEP BUFR library

Instructions for installing the NCEP BUFR library are provided below.

### Build Instructions

1. Create and navigate to the build directory:
   ```bash
   mkdir build
   cd build
   ```

2. Configure the build using CMake. The only required argument is the path to the NCEP BUFR library:
   ```bash
   cmake <OBS2IODA_ROOT_DIR> -DNCEP_BUFR_LIB=<NCEP_BUFR_LIB_PATH>
   ```

3. Build `obs2ioda-v3`:
   ```bash
   make
   ```

The `obs2ioda-v3` executable will be available in the `bin` directory inside the build directory.

---

## NCEP BUFR Library Installation

To install the NCEP BUFR library:

1. Clone the repository:
   ```bash
   git clone https://github.com/NOAA-EMC/NCEPLIBS-bufr.git
   ```

2. Create a build directory and enter it:
   ```bash
   mkdir build
   cd build
   ```

3. Configure the build using CMake (ensure NetCDF is also installed):
   ```bash
   cmake <NCEP_BUFR_ROOT_DIR>
   ```

4. Build the library:
   ```bash
   make
   ```

Make sure to note the path to the NCEP BUFR library, as it will be required when building `obs2ioda-v3`.

