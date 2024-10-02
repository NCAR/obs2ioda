module setup_mod
    use kinds, only: i_kind
    use define_mod, only: StrLen
    use core_mod, only: obs2ioda_args_t
    implicit none

    contains
        subroutine handle_user_input(obs2ioda_args, flist, ftype, nfile)

            implicit none
            type(obs2ioda_args_t), intent(inout) :: obs2ioda_args
            character(len=obs2ioda_args%NameLen), dimension(:), intent(inout) :: flist
            integer(i_kind), intent(out) :: nfile
            integer(i_kind), dimension(:), intent(out) :: ftype
            character(len=StrLen) :: tmp

            integer(i_kind)       :: iunit = 21
            integer(i_kind)       :: narg, iarg, iarg_inpdir, iarg_outdir, iarg_datetime, iarg_subsample, iarg_superob_halfwidth,&
                    ifile
            integer(i_kind)       :: itmp
            integer(i_kind)       :: iost, iret, idate
            character(len=StrLen) :: strtmp
            character(len=8)      :: subset

            narg = command_argument_count()
            ifile = 0
            obs2ioda_args%inpdir = '.'
            obs2ioda_args%outdir = '.'
            obs2ioda_args%cdatetime = ''
            flist(:) = 'null'
            iarg_inpdir = -1
            iarg_outdir = -1
            iarg_datetime = -1
            iarg_subsample = -1
            iarg_superob_halfwidth = -1
            if ( narg > 0 ) then
                do iarg = 1, narg
                    call get_command_argument(number=iarg, value=strtmp)
                    if ( trim(strtmp) == '-qc' ) then
                        obs2ioda_args%apply_gsi_qc = .true.
                    else if ( trim(strtmp) == '-noqc' ) then
                        obs2ioda_args%apply_gsi_qc = .false.
                    else if ( trim(strtmp) == '-tv' ) then
                        obs2ioda_args%do_tv_to_ts = .false.
                    else if ( trim(strtmp) == '-ahi' ) then
                        obs2ioda_args%do_ahi = .true.
                    else if ( trim(strtmp) == '-split' ) then
                        obs2ioda_args%time_split = .true.
                    else if ( trim(strtmp) == '-i' ) then
                        iarg_inpdir = iarg + 1
                    else if ( trim(strtmp) == '-o' ) then
                        iarg_outdir = iarg + 1
                    else if ( trim(strtmp) == '-t' ) then
                        iarg_datetime = iarg + 1
                    else if ( trim(strtmp) == '-s' ) then
                        iarg_subsample = iarg + 1
                    else if ( trim(strtmp) == '-superob' ) then
                        obs2ioda_args%do_superob = .true.
                        iarg_superob_halfwidth = iarg + 1
                    else
                        if ( iarg == iarg_inpdir ) then
                            call get_command_argument(number=iarg, value=tmp)
                            obs2ioda_args%inpdir = trim(tmp)
                        else if ( iarg == iarg_outdir ) then
                            call get_command_argument(number=iarg, value=tmp)
                            obs2ioda_args%outdir = trim(tmp)
                        else if ( iarg == iarg_datetime ) then
                            call get_command_argument(number=iarg, value=tmp)
                            obs2ioda_args%cdatetime = trim(tmp)
                        else if ( iarg == iarg_subsample ) then
                            call get_command_argument(number=iarg, value=strtmp)
                            if ( len_trim(strtmp) > 0 ) then
                                read(strtmp,'(i2)') obs2ioda_args%subsample
                            else
                                obs2ioda_args%subsample = 1
                            end if
                        else if ( iarg == iarg_superob_halfwidth ) then
                            call get_command_argument(number=iarg, value=strtmp)
                            if ( len_trim(strtmp) > 0 ) then
                                read(strtmp,'(i2)') obs2ioda_args%superob_halfwidth
                            else
                                obs2ioda_args%superob_halfwidth = 1
                            end if
                        else
                            ifile = ifile + 1
                            call get_command_argument(number=iarg, value=flist(ifile))
                        end if
                    end if
                end do
                if ( ifile == 0 ) then
                    nfile = obs2ioda_args%nfile_all
                    flist(:) = obs2ioda_args%flist_all(:)
                    ftype(:) = (/ obs2ioda_args%ftype_gnssro, obs2ioda_args%ftype_prepbufr, obs2ioda_args%ftype_satwnd,  &
                            obs2ioda_args%ftype_amsua, obs2ioda_args%ftype_airs, obs2ioda_args%ftype_mhs,  &
                            obs2ioda_args%ftype_iasi, obs2ioda_args%ftype_cris /)
                else
                    nfile = ifile
                end if
            else
                obs2ioda_args%inpdir = '.'
                obs2ioda_args%outdir = '.'
                nfile = obs2ioda_args%nfile_all
                flist(:) = obs2ioda_args%flist_all(:)
                ftype(:) = (/ obs2ioda_args%ftype_gnssro, obs2ioda_args%ftype_prepbufr, obs2ioda_args%ftype_satwnd,  &
                        obs2ioda_args%ftype_amsua, obs2ioda_args%ftype_airs, obs2ioda_args%ftype_mhs,  &
                        obs2ioda_args%ftype_iasi, obs2ioda_args%ftype_cris /)
            end if

            itmp = len_trim(obs2ioda_args%inpdir)
            if ( obs2ioda_args%inpdir(itmp:itmp) /= '/' ) obs2ioda_args%inpdir = trim(obs2ioda_args%inpdir)//'/'
            itmp = len_trim(obs2ioda_args%outdir)
            if ( obs2ioda_args%outdir(itmp:itmp) /= '/' ) obs2ioda_args%outdir = trim(obs2ioda_args%outdir)//'/'

            ! use default file lists if not set in command-line arguemnt
            if ( narg == 0 .or. ifile == 0 ) return

        end subroutine handle_user_input

        subroutine set_nfgat(obs2ioda_args)
            implicit none
            type(obs2ioda_args_t), intent(inout) :: obs2ioda_args
            if ( obs2ioda_args%time_split ) then
                obs2ioda_args%hour_fgat = 1  ! can also be 3 or 2
                ! corresponding to dtime_min='-3h' and dtime_max='+3h'
                obs2ioda_args%nfgat = (6/obs2ioda_args%hour_fgat) + 1
            else
                obs2ioda_args%nfgat = 1
            end if
        end subroutine set_nfgat

        subroutine parse_files_to_convert(obs2ioda_args, flist, ftype, nfile)

            implicit none
            type(obs2ioda_args_t), intent(in) :: obs2ioda_args
            character(len=obs2ioda_args%NameLen), dimension(:), intent(inout) :: flist
            integer(i_kind), intent(in) :: nfile
            integer(i_kind), dimension(:), intent(inout) :: ftype
            character(len=StrLen) :: tmp

            integer(i_kind)       :: iunit = 21
            integer(i_kind)       :: narg, iarg, iarg_inpdir, iarg_outdir, iarg_datetime, iarg_subsample, iarg_superob_halfwidth,&
                    ifile
            integer(i_kind)       :: itmp
            integer(i_kind)       :: iost, iret, idate
            character(len=StrLen) :: strtmp
            character(len=8)      :: subset

            ! determine the input file type
            fileloop: do ifile = 1, nfile
                if ( trim(flist(ifile)) == 'null' ) then
                    ftype(ifile) = obs2ioda_args%ftype_unknown
                    cycle fileloop
                end if
                open(unit=iunit, file=trim(obs2ioda_args%inpdir)//trim(flist(ifile)), form='unformatted', iostat=iost, status='old')
                call openbf(iunit, 'IN', iunit)
                call readmg(iunit,subset,idate,iret)
                !print*,subset
                if ( subset(1:5) == 'NC005' ) then
                    ftype(ifile) = obs2ioda_args%ftype_satwnd
                else
                    select case ( trim(subset) )
                    case (  'ADPUPA', 'ADPSFC' )
                        ftype(ifile) = obs2ioda_args%ftype_prepbufr
                    case ( 'NC003010' )
                        ftype(ifile) = obs2ioda_args%ftype_gnssro
                    case ( 'NC021023' )
                        ftype(ifile) = obs2ioda_args%ftype_amsua
                    case ( 'NC021027' )
                        ftype(ifile) = obs2ioda_args%ftype_mhs
                    case ( 'NC021249' )
                        ftype(ifile) = obs2ioda_args%ftype_airs
                    case ( 'NC021241' )
                        ftype(ifile) = obs2ioda_args%ftype_iasi
                    case ( 'NC021202', 'NC021206' )
                        ftype(ifile) = obs2ioda_args%ftype_cris
                    case default
                        ftype(ifile) = obs2ioda_args%ftype_unknown
                    end select
                end if
                call closbf(iunit)
                close(iunit)
            end do fileloop
        end subroutine parse_files_to_convert

        logical function input_file_exists(obs2ioda_args)
            type(obs2ioda_args_t), intent(in) :: obs2ioda_args
            logical :: fexist

            inquire(file=trim(obs2ioda_args%inpdir)//trim(obs2ioda_args%filename), exist=fexist)
            if ( .not. fexist ) then
                write(*,*) 'Warning: ',trim(obs2ioda_args%inpdir)//trim(obs2ioda_args%filename), ' not found for decoding...'
                input_file_exists = .false.
            else
                input_file_exists = .true.
            end if
        end function input_file_exists
end module setup_mod