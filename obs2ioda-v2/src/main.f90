program obs2ioda

use define_mod, only: write_nc_conv, write_nc_radiance, &
        write_nc_radiance_geo, xdata_type, StrLen
use kinds, only: i_kind
use prepbufr_mod, only: handle_prepbufr
use radiance_mod, only: handle_amsua, handle_airs, &
        handle_mhs, handle_do_radiance, &
        handle_do_radiance_hyper_ir, handle_iasi, &
        handle_cris
use gnssro_bufr2ioda, only: handle_gnssro
use ahi_hsd_mod, only: handle_do_ahi
use satwnd_mod, only: handle_satwnd
use utils_mod, only: da_advance_time
use setup_mod, only: &
        parse_files_to_convert, handle_user_input,&
        set_nfgat, input_file_exists, set_command_line_arguments
use core_mod, only: obs2ioda_args_t

implicit none

integer(i_kind)            :: ftype(8)
type(obs2ioda_args_t), target :: obs2ioda_args
character(len=1024) :: arg, cmd_line  ! adjust char len as per maximum expected cmd line length
integer :: i, num
integer, dimension(:), allocatable :: cmd_arg_indicies
character(len=StrLen), dimension(:), allocatable :: command_line_args

character (len=64) :: flist(8)  ! file names to be read in from command line arguments
integer(i_kind)         :: nfile, ifile, arg_count

arg_count = command_argument_count()
allocate(command_line_args(arg_count))

call set_command_line_arguments(command_line_args)

call handle_user_input(obs2ioda_args,flist, ftype, nfile, command_line_args)
stop
call parse_files_to_convert(obs2ioda_args, flist, ftype, nfile)

call set_nfgat(obs2ioda_args)

do ifile = 1, nfile

   obs2ioda_args%filename = flist(ifile)

   if ( ftype(ifile) == obs2ioda_args%ftype_gnssro ) then
      if ( input_file_exists(obs2ioda_args) ) then
         call handle_gnssro(obs2ioda_args)
      end if
   end if

   if ( ftype(ifile) == obs2ioda_args%ftype_satwnd ) then
      if ( input_file_exists(obs2ioda_args) ) then
         call  handle_satwnd( &
                 obs2ioda_args &
                 )
      end if
   end if

   if ( ftype(ifile) == obs2ioda_args%ftype_prepbufr ) then
      if ( input_file_exists(obs2ioda_args) ) then
         call  handle_prepbufr( &
                 obs2ioda_args &
                 )
      end if
   end if

   if ( ftype(ifile) == obs2ioda_args%ftype_amsua ) then
      if ( input_file_exists(obs2ioda_args) ) then
         call handle_amsua(obs2ioda_args)
      end if
   end if

   if ( ftype(ifile) == obs2ioda_args%ftype_airs ) then
      if ( input_file_exists(obs2ioda_args) ) then
         call handle_airs(obs2ioda_args)
      end if
   end if

   if ( ftype(ifile) == obs2ioda_args%ftype_mhs ) then
      if ( input_file_exists(obs2ioda_args) ) then
         call handle_mhs(obs2ioda_args)
      end if
   end if

end do ! nfile list

if ( obs2ioda_args%do_radiance ) then
   call  handle_do_radiance( &
              obs2ioda_args &
           )
end if

do ifile = 1, nfile

   obs2ioda_args%filename = flist(ifile)

   if ( ftype(ifile) == obs2ioda_args%ftype_iasi ) then
      if ( input_file_exists(obs2ioda_args) ) then
         call handle_iasi(obs2ioda_args)
      end if
   end if

   if ( ftype(ifile) == obs2ioda_args%ftype_cris ) then
      if ( input_file_exists(obs2ioda_args) ) then
         call handle_cris(obs2ioda_args)
      end if
   end if
end do

if ( obs2ioda_args%do_radiance_hyperIR ) then
   call  handle_do_radiance_hyper_ir( &
           obs2ioda_args &
           )
end if

if ( obs2ioda_args%do_ahi ) then
   call handle_do_ahi(obs2ioda_args)
end if

write(6,*) 'all done!'

end program obs2ioda
