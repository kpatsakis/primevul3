int cdrom_number_of_slots(struct cdrom_device_info *cdi) 
{
	int status;
	int nslots = 1;
	struct cdrom_changer_info *info;

	cd_dbg(CD_CHANGER, "entering cdrom_number_of_slots()\n");
	/* cdrom_read_mech_status requires a valid value for capacity: */
	cdi->capacity = 0; 

	info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	if ((status = cdrom_read_mech_status(cdi, info)) == 0)
		nslots = info->hdr.nslots;

	kfree(info);
	return nslots;
}
