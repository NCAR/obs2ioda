#! /bin/sh -v

FC = gfortran
BUFR_LIB = -L${BUFR_LIB_DIR} -l${BUFR_LIB_NAME}
NC_LIB = $(shell nc-config --libs)
NC_INC = $(shell nc-config --includedir)
NF_LIB = $(shell nf-config --flibs)
NF_INC = $(shell nf-config --includedir)
LIBS = ${BUFR_LIB} ${NC_LIB} ${NF_LIB}
INCS = -I${NC_INC} -I${NF_INC}
FC = gfortran
FFLAGS = -mcmodel=medium -ffree-line-length-none


#--------------
#INTEL compiler
#--------------
#FC = ifort
#BUFR_LIB = -L/glade/u/home/hclin/extlib/intel -lbufr
#FFLAGS = -mcmodel medium # needed for intel error message "failed to convert GOTPCREL relocation"
#BUFR_LIB = -L/glade/campaign/mmm/parc/ivette/pandac/converters/WRFDA_3DVAR_dmpar/var/external/bufr -lbufr
#FFLAGS = -mcmodel medium # needed for intel error message "failed to convert GOTPCREL relocation" # -g -traceback -debug all -check all

#LIBS = -L$(NETCDF)/lib -lnetcdff -lnetcdf ${BUFR_LIB}
#INCS = -I$(NETCDF)/include

OBJS = \
       define_mod.o \
       gnssro_mod.o \
       hsd.o \
       satwnd_mod.o \
       kinds.o \
       main.o \
       ncio_mod.o \
       netcdf_mod.o \
       prepbufr_mod.o \
       radiance_mod.o \
       ufo_variables_mod.o \
       utils_mod.o \
       setup_mod.o

all: obs2ioda

obs2ioda: ${OBJS}
	${FC} -o obs2ioda-v2.x ${FFLAGS} ${OBJS} ${LIBS}

kinds.o : kinds.f90
define_mod.o : define_mod.f90 kinds.o ufo_variables_mod.o
setup_mod.o : setup_mod.f90 kinds.o define_mod.o
core_mod.o : core_mod.f90 kinds.o define_mod.o
gnssro_mod.o : gnssro_mod.f90 core_mod.o
hsd.o : hsd.f90 kinds.o define_mod.o ufo_variables_mod.o ncio_mod.o utils_mod.o core_mod.o
main.o : main.f90 define_mod.f90 prepbufr_mod.o ncio_mod.o radiance_mod.o gnssro_mod.o hsd.o satwnd_mod.o setup_mod.o core_mod.o
ncio_mod.o : ncio_mod.f90 kinds.o netcdf_mod.o ufo_variables_mod.o define_mod.o
netcdf_mod.o : netcdf_mod.f90
prepbufr_mod.o : prepbufr_mod.f90 kinds.o ufo_variables_mod.o utils_mod.o define_mod.o core_mod.o ncio_mod.o
radiance_mod.o : radiance_mod.f90 kinds.o define_mod.o ufo_variables_mod.o utils_mod.o core_mod.o ncio_mod.o
satwnd_mod.o : satwnd_mod.f90 kinds.o define_mod.o ufo_variables_mod.o core_mod.o ncio_mod.o
ufo_variables_mod.o : ufo_variables_mod.F90
utils_mod.o : utils_mod.f90

.SUFFIXES : .F90 .f90 .o

.F90.o :
	${FC} ${FFLAGS} ${INCS} -c $<

.f90.o :
	${FC} ${FFLAGS} ${INCS} -c $<

%.o: %.mod

clean:
	rm -f *.o *.mod *.exe *.x
