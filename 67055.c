int sr_get_last_session(struct cdrom_device_info *cdi,
			struct cdrom_multisession *ms_info)
{
	Scsi_CD *cd = cdi->handle;

	ms_info->addr.lba = cd->ms_offset;
	ms_info->xa_flag = cd->xa_flag || cd->ms_offset > 0;

	return 0;
}
