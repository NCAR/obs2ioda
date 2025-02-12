module ncio_mod

use netcdf
use kinds, only: i_kind, r_single, r_kind, i_llong
use define_mod, only: nobtype, nvar_info, n_ncdim, n_ncgrp, nstring, ndatetime, &
   obtype_list, name_ncdim, name_ncgrp, name_var_met, name_var_info, name_sen_info, &
   xdata, itrue, ifalse, vflag, ninst, inst_list, write_nc_conv, write_nc_radiance, &
   write_nc_radiance_geo, ninst_geo, geoinst_list, &
   var_tb, nsen_info, type_var_info, type_sen_info, dim_var_info, dim_sen_info, &
   unit_var_met, iflag_conv, iflag_radiance, set_brit_obserr, set_ahi_obserr
use netcdf_mod, only: open_netcdf_for_write, close_netcdf, &
   def_netcdf_dims, def_netcdf_grp, def_netcdf_var, def_netcdf_end, &
   put_netcdf_var, get_netcdf_dims
use netcdf, only: nf90_string
use ufo_vars_mod, only: ufo_vars_getindex
use netcdf_cxx_mod

implicit none

private
public :: write_obs

contains

   function get_dim_name(dimid, nchans_nvars_flag) result(dim_name)
      integer(i_kind), intent(in) :: dimid
      logical, intent(in) :: nchans_nvars_flag
      character(nstring) :: dim_name
      character(nstring) :: tmp
      tmp = name_ncdim(dimid)
        if (nchans_nvars_flag) then
             if (trim(tmp) == 'nvars') then
                dim_name = 'nchans'
             else
                dim_name = tmp
             end if
        else
             dim_name = tmp
        end if
   end function get_dim_name

subroutine write_obs (filedate, write_opt, outdir, itim)

   implicit none

   character(len=*), intent(in)          :: filedate
   integer(i_kind),  intent(in)          :: write_opt
   character(len=*), intent(in)          :: outdir
   integer(i_kind),  intent(in)          :: itim

   character(len=512)                    :: ncfname  ! netcdf file name
   integer(i_kind), dimension(n_ncdim)   :: ncid_ncdim
   integer(i_kind), dimension(n_ncdim)   :: val_ncdim
   integer(i_kind), dimension(n_ncgrp)   :: ncid_ncgrp
   character(len=nstring)                :: ncname
   integer(i_kind)                       :: ncfileid
   integer(i_kind)                       :: ntype
   integer(i_kind)                       :: i, ityp, igrp, ivar, ii, iv, jj
   integer(i_kind)                       :: idim, dim1, dim2
   character(len=nstring),   allocatable :: str_nstring(:)
   character(len=ndatetime), allocatable :: str_ndatetime(:)
   character(len=nstring)                :: str_tmp
   integer(i_kind)                       :: iflag
   integer(i_kind), allocatable :: ichan(:)
   real(r_kind),    allocatable :: rtmp2d(:,:)
   real(r_kind),    allocatable :: obserr(:)
   integer(i_kind) :: imin_datetime(1), imax_datetime(1)
   integer(i_kind) :: ncstatus
   integer(i_kind) :: has_wavenumber
   integer(i_kind) :: ncid_ncgrp_wn
   integer(i_kind) :: status, netcdfID
   logical :: nchans_nvars_flag
   character(len=nstring) :: dim1_name
    character(len=nstring) :: dim2_name

   if ( write_opt == write_nc_conv ) then
      ntype = nobtype
   else if ( write_opt == write_nc_radiance ) then
      ntype = ninst
   else if ( write_opt == write_nc_radiance_geo ) then
      ntype = ninst_geo
   else
      write(*,*) ' Error: unknwon write_opt = ', write_opt
      return
   end if

   iv = ufo_vars_getindex(name_ncdim, 'nstring')
   if ( iv > 0 ) val_ncdim(iv) = nstring
   iv = ufo_vars_getindex(name_ncdim, 'ndatetime')
   if ( iv > 0 ) val_ncdim(iv) = ndatetime

   obtype_loop: do ityp = 1, ntype

      if ( xdata(ityp,itim)%nlocs == 0 ) cycle obtype_loop

      iv = ufo_vars_getindex(name_var_info, 'dateTime')
      imin_datetime = minloc(xdata(ityp,itim)%xinfo_int64(:,iv))
      imax_datetime = maxloc(xdata(ityp,itim)%xinfo_int64(:,iv))
      iv = ufo_vars_getindex(name_var_info, 'datetime')
      xdata(ityp,itim)%min_datetime = xdata(ityp,itim)%xinfo_char(imin_datetime(1),iv)
      xdata(ityp,itim)%max_datetime = xdata(ityp,itim)%xinfo_char(imax_datetime(1),iv)

      if ( write_opt == write_nc_conv ) then
         ncfname = trim(outdir)//trim(obtype_list(ityp))//'_obs_'//trim(filedate)//'.h5'
      else if ( write_opt == write_nc_radiance ) then
         ncfname = trim(outdir)//trim(inst_list(ityp))//'_obs_'//trim(filedate)//'.h5'
      else if ( write_opt == write_nc_radiance_geo ) then
         ncfname = trim(outdir)//trim(geoinst_list(ityp))//'_obs_'//trim(filedate)//'.h5'
      end if
      if ( write_opt == write_nc_radiance .or. write_opt == write_nc_radiance_geo ) then
         iv = ufo_vars_getindex(name_sen_info, 'sensor_channel')
         allocate (ichan(xdata(ityp,itim)%nvars))
         ichan(:) = xdata(ityp,itim)%xseninfo_int(:,iv)
         allocate (obserr(xdata(ityp,itim)%nvars))
         if  ( write_opt == write_nc_radiance_geo ) then
             call set_ahi_obserr(geoinst_list(ityp), xdata(ityp,itim)%nvars, obserr)
         else
             call set_brit_obserr(inst_list(ityp), xdata(ityp,itim)%nvars, obserr)
         end if
      end if
      write(*,*) '--- writing ', trim(ncfname)
      status = netcdfCreate(trim(ncfname), netcdfID)
      iv = ufo_vars_getindex(name_ncdim, 'nvars')
      val_ncdim(iv) = xdata(ityp,itim)%nvars
      iv = ufo_vars_getindex(name_ncdim, 'nlocs')
      val_ncdim(iv) = xdata(ityp,itim)%nlocs

      ! define netcdf dimensions
      if ( write_opt == write_nc_conv ) then
         ncname = 'nvars'
         nchans_nvars_flag = .false.
      else if ( write_opt == write_nc_radiance .or. write_opt == write_nc_radiance_geo ) then
         ncname = 'nchans'
            nchans_nvars_flag = .true.
      end if

      ncid_ncdim(1) = 1
      status = netcdfAddDim(netcdfID, trim(ncname), val_ncdim(1))
      status = netcdfPutAtt(netcdfID, trim(ncname), val_ncdim(1))
      status = netcdfAddVar(netcdfID, trim(ncname), NF90_INT, 1, [trim(ncname)])
        status = netcdfPutAtt(netcdfID, "suggested_chunk_dim", val_ncdim(1), varName=trim(ncname))
      do i = 2, n_ncdim
         ncid_ncdim(i) = i
         status = netcdfAddDim(netcdfID, trim(name_ncdim(i)), val_ncdim(i))
         status = netcdfPutAtt(netcdfID, trim(name_ncdim(i)), val_ncdim(i))
         status = netcdfAddVar(netcdfID, trim(name_ncdim(i)), NF90_INT, 1, [trim(name_ncdim(i))])
         status = netcdfPutAtt(netcdfID, "suggested_chunk_dim", 100, varName=trim(name_ncdim(i)))
      end do

      ! define global attributes
      status = netcdfPutAtt(netcdfID, "min_datetime", xdata(ityp,itim)%min_datetime)
      status = netcdfPutAtt(netcdfID, "max_datetime", xdata(ityp,itim)%max_datetime)

      if ( allocated(xdata(ityp,itim)%wavenumber) ) then
         has_wavenumber = itrue
      else
         has_wavenumber = ifalse
      end if

      ! define netcdf groups
      do i = 1, n_ncgrp
         if ( write_opt == write_nc_radiance .or. write_opt == write_nc_radiance_geo ) then
            if ( trim(name_ncgrp(i)) == 'ObsType' ) cycle
         end if
         status = netcdfAddGroup(netcdfID, trim(name_ncgrp(i)))
      end do
      if ( has_wavenumber == itrue ) then
         ! use deprecated VarMetaData group for wavenumber before related code in UFO is updated
         status = netcdfAddGroup(netcdfID, 'VarMetaData')
      end if

      ! define netcdf variables
      if ( write_opt == write_nc_conv ) then
         do i = 1, xdata(ityp,itim) % nvars
            ivar = xdata(ityp,itim) % var_idx(i)
            ncname = trim(name_var_met(ivar))
            igrp = ufo_vars_getindex(name_ncgrp, 'ObsValue')
            dim1_name = get_dim_name(ncid_ncdim(2), nchans_nvars_flag)
            status = netcdfAddVar(netcdfID, ncname, NF90_FLOAT, 1, [dim1_name], "ObsValue")
            print*, "ncname: ", ncname
            status = netcdfSetFill(netcdfID, ncname, 1, -999.0, "ObsValue")
            status = netcdfPutAtt(netcdfID, "units", trim(unit_var_met(ivar)), varName=ncname, groupName="ObsValue")
            igrp = ufo_vars_getindex(name_ncgrp, 'ObsError')
            dim1_name = get_dim_name(ncid_ncdim(2), nchans_nvars_flag)
            status = netcdfAddVar(netcdfID, ncname, NF90_FLOAT, 1, [dim1_name], "ObsError")
            status = netcdfSetFill(netcdfID, ncname, 1, -999.0, "ObsError")
            status = netcdfPutAtt(netcdfID, "units", trim(unit_var_met(ivar)), varName=ncname, groupName="ObsError")
            igrp = ufo_vars_getindex(name_ncgrp, 'PreQC')
            dim1_name = get_dim_name(ncid_ncdim(2), nchans_nvars_flag)
            status = netcdfAddVar(netcdfID, ncname, NF90_INT, 1, [dim1_name], "PreQC")
            status = netcdfSetFill(netcdfID, ncname, 1, -999, "PreQC")
            igrp = ufo_vars_getindex(name_ncgrp, 'ObsType')
            dim1_name = get_dim_name(ncid_ncdim(2), nchans_nvars_flag)
            status = netcdfAddVar(netcdfID, ncname, NF90_INT, 1, [dim1_name], "ObsType")
            status = netcdfSetFill(netcdfID, ncname, 1, -999, "ObsType")
         end do
      else if ( write_opt == write_nc_radiance .or. write_opt == write_nc_radiance_geo ) then
         ncname = trim(var_tb)
         idim = ufo_vars_getindex(name_ncdim, 'nvars') ! note that its ncname is actually nchans
         dim1 = ncid_ncdim(idim)
         idim = ufo_vars_getindex(name_ncdim, 'nlocs')
         dim2 = ncid_ncdim(idim)
         igrp = ufo_vars_getindex(name_ncgrp, 'ObsValue')
         dim1_name = get_dim_name(ncid_ncdim(dim1), nchans_nvars_flag)
         dim2_name = get_dim_name(ncid_ncdim(dim2), nchans_nvars_flag)
         status = netcdfAddVar(netcdfID, ncname, NF90_FLOAT, 2, &
                                 [dim2_name, dim1_name], "ObsValue")
         status = netcdfSetFill(netcdfID, ncname, 1, -999.0, "ObsValue")
         status = netcdfPutAtt(netcdfID, "units", "K", varName=ncname, groupName="ObsValue")
         igrp = ufo_vars_getindex(name_ncgrp, 'ObsError')
         dim1_name = get_dim_name(ncid_ncdim(dim1), nchans_nvars_flag)
         dim2_name = get_dim_name(ncid_ncdim(dim2), nchans_nvars_flag)
         status = netcdfAddVar(netcdfID, ncname, NF90_FLOAT, 2, &
                                 [dim2_name, dim1_name], "ObsError")
         status = netcdfSetFill(netcdfID, ncname, 1, -999.0, "ObsError")
         status = netcdfPutAtt(netcdfID, "units", "K", varName=ncname, groupName="ObsError")
         igrp = ufo_vars_getindex(name_ncgrp, 'PreQC')
         dim1_name = get_dim_name(ncid_ncdim(dim1), nchans_nvars_flag)
         dim2_name = get_dim_name(ncid_ncdim(dim2), nchans_nvars_flag)
         status = netcdfAddVar(netcdfID, ncname, NF90_INT, 2, &
                                 [dim2_name, dim1_name], "PreQC")
         status = netcdfSetFill(netcdfID, ncname, 1, -999, "PreQC")
      end if

      var_info_def_loop: do i = 1, nvar_info
         if ( write_opt == write_nc_conv ) then
            iflag = iflag_conv(i,ityp)
         else if ( write_opt == write_nc_radiance .or. write_opt == write_nc_radiance_geo ) then
            iflag = iflag_radiance(i)
         end if
         if ( iflag /= itrue ) cycle var_info_def_loop
         ncname = trim(name_var_info(i))
         igrp = ufo_vars_getindex(name_ncgrp, 'MetaData')
         idim = ufo_vars_getindex(name_ncdim, dim_var_info(1,i))
         dim1 = ncid_ncdim(idim)
         if ( ufo_vars_getindex(name_ncdim, dim_var_info(2,i)) > 0 ) then
            idim = ufo_vars_getindex(name_ncdim, dim_var_info(2,i))
            dim2 = ncid_ncdim(idim)
!            call def_netcdf_var(ncid_ncgrp(igrp),ncname,(/dim1,dim2/),type_var_info(i))
            dim1_name = get_dim_name(ncid_ncdim(dim2), nchans_nvars_flag)
            status = netcdfAddVar(netcdfID, ncname, NF90_STRING, 1, &
                                 [dim1_name], "MetaData")
            status = netcdfSetFill(netcdfID, ncname, 1, "", "MetaData")
         else
            if ( ncname == 'dateTime' ) then
               dim1_name = get_dim_name(ncid_ncdim(dim1), nchans_nvars_flag)
               status = netcdfAddVar(netcdfID, ncname, type_var_info(i), 1, &
                  [dim1_name], "MetaData")
               status = netcdfPutAtt(netcdfID, "units", "seconds since 1970-01-01T00:00:00Z", varName=ncname, &
                    groupName="MetaData")
               if (type_var_info(i) == NF90_INT) then
                    status = netcdfSetFill(netcdfID, ncname, 1, -999, "MetaData")
               else if (type_var_info(i) == NF90_INT64) then
                 status = netcdfSetFill(netcdfID, ncname, 1, -999_i_llong, "MetaData")
               else if (type_var_info(i) == NF90_FLOAT) then
                    status = netcdfSetFill(netcdfID, ncname, 1, -999.0, "MetaData")
               end if
            else
               dim1_name = get_dim_name(ncid_ncdim(dim1), nchans_nvars_flag)
               status = netcdfAddVar(netcdfID, ncname, type_var_info(i), 1, &
                    [dim1_name], "MetaData")
               if (type_var_info(i) == NF90_INT) then
                    status = netcdfSetFill(netcdfID, ncname, 1, -999, "MetaData")
               else if (type_var_info(i) == NF90_FLOAT) then
                    status = netcdfSetFill(netcdfID, ncname, 1, -999.0, "MetaData")
               end if
            end if
         end if
      end do var_info_def_loop ! nvar_info

      if ( write_opt == write_nc_radiance .or. write_opt == write_nc_radiance_geo ) then
         do i = 1, nsen_info
            ncname = trim(name_sen_info(i))
            igrp = ufo_vars_getindex(name_ncgrp, 'MetaData')
            idim = ufo_vars_getindex(name_ncdim, dim_sen_info(1,i))
            dim1 = ncid_ncdim(idim)
            if ( ufo_vars_getindex(name_ncdim, dim_sen_info(2,i)) > 0 ) then
               idim = ufo_vars_getindex(name_ncdim, dim_sen_info(2,i))
               dim2 = ncid_ncdim(idim)
               dim1_name = get_dim_name(ncid_ncdim(dim1), nchans_nvars_flag)
               dim2_name = get_dim_name(ncid_ncdim(dim2), nchans_nvars_flag)
               status = netcdfAddVar(netcdfID, ncname, type_sen_info(i), 2, &
                                    [dim2_name, dim1_name], "MetaData")
               if (type_sen_info(i) == NF90_INT) then
                    status = netcdfSetFill(netcdfID, ncname, 1, -999, "MetaData")
               else if (type_sen_info(i) == NF90_FLOAT) then
                  status = netcdfSetFill(netcdfID, ncname, 1, -999.0, "MetaData")
               end if
            else
               dim1_name = get_dim_name(ncid_ncdim(dim1), nchans_nvars_flag)
               status = netcdfAddVar(netcdfID, ncname, type_sen_info(i), 1, &
                                        [dim1_name], "MetaData")
                if (type_sen_info(i) == NF90_INT) then
                    status = netcdfSetFill(netcdfID, ncname, 1, -999, "MetaData")
                else if (type_sen_info(i) == NF90_FLOAT) then
                    status = netcdfSetFill(netcdfID, ncname, 1, -999.0, "MetaData")
               end if
            end if
         end do ! nsen_info
         if ( has_wavenumber == itrue ) then
            idim = ufo_vars_getindex(name_ncdim, 'nvars')
            dim1 = ncid_ncdim(idim)
            dim1_name = get_dim_name(ncid_ncdim(dim1), nchans_nvars_flag)
            status = netcdfAddVar(netcdfID, 'sensor_band_central_radiation_wavenumber', NF90_FLOAT, 1, &
                                 [dim1_name], "MetaData")
            status = netcdfSetFill(netcdfID, 'sensor_band_central_radiation_wavenumber', 1, -999.0, "MetaData")
         end if
      end if ! write_nc_radiance


      ! writing netcdf variables
      if ( write_opt == write_nc_conv ) then
         var_loop: do i = 1, xdata(ityp,itim) % nvars
            ivar = xdata(ityp,itim) % var_idx(i)
            if ( vflag(ivar,ityp) == itrue ) then
               ncname = trim(name_var_met(ivar))
               igrp = ufo_vars_getindex(name_ncgrp, 'ObsValue')
               status = netcdfPutVar(netcdfID, ncname, [xdata(ityp,itim)%xfield(:,i)%val], "ObsValue")
               ncname = trim(name_var_met(ivar))
               igrp = ufo_vars_getindex(name_ncgrp, 'ObsError')
               status = netcdfPutVar(netcdfID, ncname, [xdata(ityp,itim)%xfield(:,i)%err], "ObsError")
               ncname = trim(name_var_met(ivar))
               igrp = ufo_vars_getindex(name_ncgrp, 'PreQC')
               status = netcdfPutVar(netcdfID, ncname, [xdata(ityp,itim)%xfield(:,i)%qm], "PreQC")
               ncname = trim(name_var_met(ivar))
               igrp = ufo_vars_getindex(name_ncgrp, 'ObsType')
               status = netcdfPutVar(netcdfID, ncname, [xdata(ityp,itim)%xfield(:,i)%rptype], "ObsType")
            end if
         end do var_loop
      else if ( write_opt == write_nc_radiance .or. write_opt == write_nc_radiance_geo ) then
         ncname = "nchans"
         status = netcdfPutVar(netcdfID, ncname, ichan(:))
         allocate(rtmp2d(xdata(ityp,itim)%nvars, xdata(ityp,itim)%nlocs))
         ncname = trim(var_tb)
         igrp = ufo_vars_getindex(name_ncgrp, 'ObsValue')
         do jj = 1, xdata(ityp,itim)%nvars
            do ii = 1, xdata(ityp,itim)%nlocs
               rtmp2d(jj,ii) = xdata(ityp,itim)%xfield(ii,jj)%val
            end do
         end do
            status = netcdfPutVar(netcdfID, ncname, &
                reshape(rtmp2d(:,:), [xdata(ityp, itim)%nvars*xdata(ityp, itim)%nlocs]),&
                "ObsValue")
         igrp = ufo_vars_getindex(name_ncgrp, 'ObsError')
         do ii = 1, xdata(ityp,itim)%nlocs
            rtmp2d(:,ii) = obserr(:)
         end do
         status = netcdfPutVar(netcdfID, ncname, &
                 reshape(rtmp2d(:,:), [xdata(ityp, itim)%nvars*xdata(ityp, itim)%nlocs]),&
                 "ObsError")
         igrp = ufo_vars_getindex(name_ncgrp, 'PreQC')
         do jj = 1, xdata(ityp,itim)%nvars
            do ii = 1, xdata(ityp,itim)%nlocs
               rtmp2d(jj,ii) = xdata(ityp,itim)%xfield(ii,jj)%qm
            end do
         end do
         status = netcdfPutVar(netcdfID, ncname, &
            reshape(rtmp2d(:,:), [xdata(ityp, itim)%nvars*xdata(ityp, itim)%nlocs]),&
            "PreQC")
         deallocate(rtmp2d)
      end if

      var_info_loop: do i = 1, nvar_info
         if ( write_opt == write_nc_conv ) then
            iflag = iflag_conv(i,ityp)
         else if ( write_opt == write_nc_radiance .or. write_opt == write_nc_radiance_geo ) then
            iflag = iflag_radiance(i)
         end if
         if ( iflag /= itrue ) cycle var_info_loop
         ncname = trim(name_var_info(i))
         igrp = ufo_vars_getindex(name_ncgrp, 'MetaData')
         if ( type_var_info(i) == nf90_int ) then
            status = netcdfPutVar(netcdfID, ncname, xdata(ityp,itim)%xinfo_int(:,i), "MetaData")
         else if ( type_var_info(i) == nf90_float ) then
            status = netcdfPutVar(netcdfID, ncname, xdata(ityp,itim)%xinfo_float(:,i), "MetaData")
         else if ( type_var_info(i) == nf90_char ) then
            if ( trim(name_var_info(i)) == 'variable_names' ) then
               if ( write_opt == write_nc_conv ) then
                  status = netcdfPutVar(netcdfID, ncname, name_var_met(xdata(ityp,itim)%var_idx(:)), "MetaData")
               end if
            else if ( trim(name_var_info(i)) == 'station_id' ) then
               allocate(str_nstring(xdata(ityp,itim)%nlocs))
               str_nstring(:) = xdata(ityp,itim)%xinfo_char(:,i)
               status = netcdfPutVar(netcdfID, ncname, str_nstring, "MetaData")
               deallocate(str_nstring)
            else if ( trim(name_var_info(i)) == 'datetime' ) then
               allocate(str_ndatetime(xdata(ityp,itim)%nlocs))
               do ii = 1, xdata(ityp,itim)%nlocs
                  str_tmp = xdata(ityp,itim)%xinfo_char(ii,i)
                  str_ndatetime(ii) = str_tmp(1:ndatetime)
               end do
               call put_netcdf_var(ncid_ncgrp(igrp),ncname,str_ndatetime)
                status = netcdfPutVar(netcdfID, ncname, str_ndatetime, "MetaData")
               deallocate(str_ndatetime)
            end if
         else if ( type_var_info(i) == nf90_int64 ) then
            status = netcdfPutVar(netcdfID, ncname, xdata(ityp,itim)%xinfo_int64(:,i), "MetaData")
         end if
      end do var_info_loop

      if ( write_opt == write_nc_radiance .or. write_opt == write_nc_radiance_geo ) then
         do i = 1, nsen_info
            ncname = trim(name_sen_info(i))
            igrp = ufo_vars_getindex(name_ncgrp, 'MetaData')
            if ( type_sen_info(i) == nf90_int ) then
               status = netcdfPutVar(netcdfID, ncname, xdata(ityp,itim)%xseninfo_int(:,i), "MetaData")
            else if ( type_sen_info(i) == nf90_float ) then
                status = netcdfPutVar(netcdfID, ncname, xdata(ityp,itim)%xseninfo_float(:,i), "MetaData")
            else if ( type_sen_info(i) == nf90_char ) then
                status = netcdfPutVar(netcdfID, ncname, xdata(ityp,itim)%xseninfo_char(:,i), "MetaData")
            end if
         end do
         if ( has_wavenumber == itrue ) then
            status = netcdfPutVar(netcdfID, 'sensor_band_central_radiation_wavenumber', &
                                xdata(ityp,itim)%wavenumber(:), "MetaData")
         end if
         deallocate (ichan)
         deallocate (obserr)
      end if ! write_nc_radiance

      status = netcdfClose(netcdfID)

   end do obtype_loop

   ! deallocate xdata
   do i = 1, ntype
      if ( allocated(xdata(i,itim)%var_idx) )        deallocate(xdata(i,itim)%var_idx)
      if ( allocated(xdata(i,itim)%xfield) )         deallocate(xdata(i,itim)%xfield)
      if ( allocated(xdata(i,itim)%xinfo_int) )      deallocate(xdata(i,itim)%xinfo_int)
      if ( allocated(xdata(i,itim)%xinfo_int64) )    deallocate(xdata(i,itim)%xinfo_int64)
      if ( allocated(xdata(i,itim)%xinfo_float) )    deallocate(xdata(i,itim)%xinfo_float)
      if ( allocated(xdata(i,itim)%xinfo_char) )     deallocate(xdata(i,itim)%xinfo_char)
      if ( allocated(xdata(i,itim)%xseninfo_int) )   deallocate(xdata(i,itim)%xseninfo_int)
      if ( allocated(xdata(i,itim)%xseninfo_float) ) deallocate(xdata(i,itim)%xseninfo_float)
      if ( allocated(xdata(i,itim)%xseninfo_char) )  deallocate(xdata(i,itim)%xseninfo_char)
      if ( allocated(xdata(i,itim)%wavenumber) )     deallocate(xdata(i,itim)%wavenumber)
   end do
!   deallocate(xdata) ! moved to main.f90

end subroutine write_obs

end module ncio_mod
