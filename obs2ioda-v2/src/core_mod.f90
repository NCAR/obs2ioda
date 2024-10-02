module core_mod
    use define_mod, only: write_nc_radiance, xdata_type, write_nc_conv, &
        write_nc_radiance_geo
    use kinds, only: i_kind
    private
    integer(i_kind), parameter :: NameLen_   = 64
    integer(i_kind), parameter :: DateLen_   = 10
    integer(i_kind), parameter :: DateLen14_ = 14
    integer(i_kind), parameter :: nfile_all_ = 8
    public :: obs2ioda_args_t
    type obs2ioda_args_t
        character(len=NameLen_) :: filename
        character(len=DateLen_) :: filedate
        character(len=DateLen_) :: filedate_out
        integer(i_kind) :: nfgat
        integer(i_kind) :: hour_fgat
        type(xdata_type), allocatable, dimension(:,:) :: xdata  ! dim 1: number of ob types
        logical :: do_radiance = .false.
        logical :: do_radiance_hyperIR = .false.
        logical :: apply_gsi_qc = .true.
        logical :: do_tv_to_ts = .true.
        logical :: do_ahi = .false.
        logical :: time_split = .false.
        logical :: do_superob = .false.
        character(len=:), allocatable :: inpdir
        character(len=:), allocatable :: outdir
        character(len=:), allocatable :: cdatetime
        integer(i_kind) :: subsample
        integer(i_kind) :: superob_halfwidth
        integer(i_kind) :: NameLen   = NameLen_
        integer(i_kind) :: DateLen   = DateLen_
        integer(i_kind) :: DateLen14 = DateLen14_
        integer(i_kind) :: nfile_all = nfile_all_
        integer(i_kind) :: ftype_unknown  = -1
        integer(i_kind) :: ftype_prepbufr =  1
        integer(i_kind) :: ftype_gnssro   =  2
        integer(i_kind) :: ftype_amsua    =  3
        integer(i_kind) :: ftype_mhs      =  4
        integer(i_kind) :: ftype_airs     =  5
        integer(i_kind) :: ftype_satwnd   =  6
        integer(i_kind) :: ftype_iasi     =  7
        integer(i_kind) :: ftype_cris     =  8
        integer(i_kind) :: write_nc_radiance_geo = write_nc_radiance_geo
        integer(i_kind) :: write_nc_radiance     = write_nc_radiance
        integer(i_kind) :: write_nc_conv         = write_nc_conv
        character(len=NameLen_) :: flist
        character(len=NameLen_)     :: flist_all(nfile_all_) = &
                (/                    &
                        "gnssro.bufr    ", &
                                "prepbufr.bufr  ", &
                                "satwnd.bufr    ", &
                                "amsua.bufr     ", &
                                "airs.bufr      ", &
                                "mhs.bufr       ", &
                                "iasi.bufr      ", &
                                "cris.bufr      "  &
                        /)
end type obs2ioda_args_t
end module core_mod