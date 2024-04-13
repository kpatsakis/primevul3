int sr_lock_door(struct cdrom_device_info *cdi, int lock)
{
	Scsi_CD *cd = cdi->handle;

	return scsi_set_medium_removal(cd->device, lock ?
		       SCSI_REMOVAL_PREVENT : SCSI_REMOVAL_ALLOW);
}
