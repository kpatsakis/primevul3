static int cdrom_slot_status(struct cdrom_device_info *cdi, int slot)
{
	struct cdrom_changer_info *info;
	int ret;

	cd_dbg(CD_CHANGER, "entering cdrom_slot_status()\n");
	if (cdi->sanyo_slot)
		return CDS_NO_INFO;
	
	info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	if ((ret = cdrom_read_mech_status(cdi, info)))
		goto out_free;

	if (info->slots[slot].disc_present)
		ret = CDS_DISC_OK;
	else
		ret = CDS_NO_DISC;

out_free:
	kfree(info);
	return ret;
}
