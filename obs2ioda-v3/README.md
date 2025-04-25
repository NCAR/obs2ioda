# obs2ioda-v3
## Installation
obs2ioda-v3 utilizes CMake as its primary build system. Follow the steps below to build the project:

### Prerequisites
Please make sure the following libraries are installed:
- NetCDF
- NCEP BUFR library. (Instructions for installing the NCEP BUFR library are provided in a subsequent section)

If you have an environment preconfigured for `mpas-jedi`, simply source that environment prior to building `obs2ioda`.

### Build Instructions
1. First, clone the repository into your preferred directory (`<OBS2IODA_ROOT_DIR>`):
   ```bash
   git clone https://github.com/NCAR/obs2ioda.git <OBS2IODA_ROOT_DIR>
   ```
2. Create a new directory `build` and navigate into it:
   ```bash
   mkdir build && cd build
   ```
3. Locate the NCEP BUFR library by executing the following command in the `NCEP BUFR` library's build directory:
   ```bash
   find . -name *libbufr*
   ```
4. Next, run CMake to configure the build. Specify the `CMAKE_BUILD_TYPE` option to set the build type. Currently, the supported types are `Release`, `RelWithDebInfo`, and `Debug`. Don't forget to include the path to the NCEP BUFR library:
   ```bash
   cmake <OBS2IODA_ROOT_DIR> -DNCEP_BUFR_LIB=<NCEP_BUFR_LIB_PATH> -DCMAKE_BUILD_TYPE=<BUILD_TYPE>
   ```
5. Finally, build `obs2ioda` using `CMake`'s build tool. In this case, we use `GNU Make`, but other build tools supported by `CMake` can be used:
   ```bash
   make
   ```
The `obs2ioda-v3` executable will reside in the `bin` directory within the build directory.

---
## Installing NCEP BUFR Library
To install the NCEP BUFR library, follow these steps:

1. Clone the NCEP BUFR repository into a directory of your choice (`<NCEP_BUFR_ROOT_DIR>`):
   ```bash
   git clone https://github.com/NOAA-EMC/NCEPLIBS-bufr.git <NCEP_BUFR_ROOT_DIR>
   ```
2. Create a new directory `build` and navigate into it:
   ```bash
   mkdir build && cd build
   ```
3. Run CMake to configure the build (Ensure NetCDF is installed):
   ```bash
   cmake <NCEP_BUFR_ROOT_DIR>
   ```
4. Build the library with the command:
   ```bash
   make
   ```
5. To locate the NCEP BUFR library, run:
   ```bash
   find . -name *libbufr*
   ```
Remember to note down the library path (`<NCEP_BUFR_LIB_PATH>`) required for the build process of `obs2ioda-v3`.


## Converting PREPBUFR and BUFR files
```
Usage: obs2ioda-v3 [-i input_dir] [-o output_dir] [bufr_filename(s)_to_convert] [-split]
```
If [-i input_dir] [-o output_dir] are not specified in the command line, the default is the current working directory.  
If [bufr_filename(s)_to_convert] is not specified in the command line, the code looks for file name, **prepbufr.bufr** (also **satwnd.bufr**, **gnssro.bufr**, **amsua.bufr**, **airs.bufr**, **mhs.bufr**, **iasi.bufr**, **cris.bufr**), in the input/working directory. If the file exists, do the conversion, otherwise skip it.  
If specify ``-split``, the converted file will contain hourly data.

> obs2ioda-v3 -i input_dir -o output_dir prepbufr.gdas.YYYYMMDD.tHHz.nr

Example output files (date in the output filename is extracted from the input bufr files):  
aircraft_obs_YYYYMMDDHH.h5  
ascat_obs_YYYYMMDDHH.h5  
profiler_obs_YYYYMMDDHH.h5  
satwind_obs_YYYYMMDDHH.h5  
sfc_obs_YYYYMMDDHH.h5  
sondes_obs_YYYYMMDDHH.h5

> obs2ioda-v3 -i input_dir -o output_dir gdas.satwnd.tHHz.YYYYMMDD.bufr

Example output files (date in the output filename is extracted from the input bufr files):  
satwnd_obs_YYYYMMDDHH.h5  (GOES-16/GOES-17, AVHRR (METOP/NOAA), VIIRS (NPP/NOAA), LEOGEO AMVs)

> obs2ioda-v3 -i input_dir -o output_dir gdas.1bamua.tHHz.YYYYMMDD.bufr

Example output files:  
amsua_metop-a_obs_YYYYMMDDHH.h5  
amsua_metop-b_obs_YYYYMMDDHH.h5  
amsua_n15_obs_YYYYMMDDHH.h5  
amsua_n18_obs_YYYYMMDDHH.h5  
amsua_n19_obs_YYYYMMDDHH.h5

> obs2ioda-v3 -i input_dir -o output_dir gdas.airsev.tHHz.YYYYMMDD.bufr

Example output files:  
amsua_aqua_obs_YYYYMMDDHH.h5

> obs2ioda-v3 -i input_dir -o output_dir gdas.1bmhs.tHHz.YYYYMMDD.bufr

Example output files:  
mhs_metop-a_obs_YYYYMMDDHH.h5  
mhs_metop-b_obs_YYYYMMDDHH.h5  
mhs_n18_obs_YYYYMMDDHH.h5  
mhs_n19_obs_YYYYMMDDHH.h5

> obs2ioda-v3 -i input_dir -o output_dir gdas.mtiasi.tHHz.YYYYMMDD.bufr

**the following CRTM SpcCoeff files in little_endian must be present in the working directory for IASI radiance to brightness temperature conversion**  
iasi_metop-a.SpcCoeff.bin -> iasi616_metop-a.SpcCoeff.bin  
iasi_metop-b.SpcCoeff.bin -> iasi616_metop-b.SpcCoeff.bin  
iasi_metop-c.SpcCoeff.bin -> iasi616_metop-c.SpcCoeff.bin

Example output files:  
iasi_metop-a_obs_YYYYMMDDHH.h5  
iasi_metop-b_obs_YYYYMMDDHH.h5  
iasi_metop-c_obs_YYYYMMDDHH.h5

> obs2ioda-v3 -i input_dir -o output_dir gdas.crisf4.tHHz.YYYYMMDD.bufr

**the following CRTM SpcCoeff files in little_endian must be present in the working directory for CrIS radiance to brightness temperature conversion**  
_for **cris** bufr file_  
cris_npp.SpcCoeff.bin -> cris399_npp.SpcCoeff.bin  
cris_n20.SpcCoeff.bin -> cris399_n20.SpcCoeff.bin  
_for **crisf4** bufr file_  
cris_npp.SpcCoeff.bin -> cris-fsr431_npp.SpcCoeff.bin  
cris_n20.SpcCoeff.bin -> cris-fsr431_n20.SpcCoeff.bin

Example output files:  
cris_npp_obs_YYYYMMDDHH.h5  
cris_n20_obs_YYYYMMDDHH.h5

> obs2ioda-v3 -i input_dir -o output_dir gdas.gpsro.tHHz.YYYYMMDD.bufr

Example output files:  
gnssro_obs_YYYYMMDDHH.h5

## Converting Himawari Standard Data (HSD) FLDK files
```
Usage: obs2ioda-v3 -i input_dir -ahi -t YYYYMMDDHHNN [-s 1]
```

Input files are a list of Himawari Standard Data, e.g. HS_H08_20200815_0000_B14_FLDK_R20_S0210.DAT in the input_dir.  
Minute must be specified in the time (-t) option.  
Number of pixels to skip must be specified in the (-s) option. The default value of 1 means no pixels will be skipped.

## Notes
* The output prefix (before _obs) is defined in define_mod.f90
* The mapping of numeric report types to the named types is coded in define_mod.f90
  through subroutines set_obtype_conv, set_name_satellite, set_name_sensor.
* For gdas.satwnd.tHHz.YYYYMMDD.bufr, only GOES-16/GOES-17, AVHRR (METOP/NOAA), VIIRS (NPP/NOAA), LEOGEO AMVs are converted when available. Other AMVs are available through PREPBUFR files.

## The current version is coded to match current GSI-processed diags as close as possible.
* The ob errors of conventional observations are either extracted from the input prepbufr or from an external error table (if obs_errtable exists in the working directory).
* The ob errors of AMSU-A/MHS radiances are coded in define_mod.f90. This should be changed in the future to read in from an external error table.
* The ob errors of satwnd-decoded AMVs are from an external error table (obs_errtable).
* Subroutine filter_obs_conv applies some additional QC for conventional observations as in GSI's read_prepbufr.f90 for the global model and can be de-activated through ``-noqc`` command-line option.
  100 is added to the @PreQC value when the ob is flagged as not-use by filter_obs_conv.  
  100 is chosen to make the original prepbufr quality marker easily readable.
* Subroutine filter_obs_satwnd applies QC for GOES-16/GOES-17 AMVs as in GSI's read_satwnd.f90.  
  @PreQC value is set to 15 for rejected obs.
