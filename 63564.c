static int cdrom_open_write(struct cdrom_device_info *cdi)
{
	int mrw, mrw_write, ram_write;
	int ret = 1;

	mrw = 0;
	if (!cdrom_is_mrw(cdi, &mrw_write))
		mrw = 1;

	if (CDROM_CAN(CDC_MO_DRIVE))
		ram_write = 1;
	else
		(void) cdrom_is_random_writable(cdi, &ram_write);
	
	if (mrw)
		cdi->mask &= ~CDC_MRW;
	else
		cdi->mask |= CDC_MRW;

	if (mrw_write)
		cdi->mask &= ~CDC_MRW_W;
	else
		cdi->mask |= CDC_MRW_W;

	if (ram_write)
		cdi->mask &= ~CDC_RAM;
	else
		cdi->mask |= CDC_RAM;

	if (CDROM_CAN(CDC_MRW_W))
		ret = cdrom_mrw_open_write(cdi);
	else if (CDROM_CAN(CDC_DVD_RAM))
		ret = cdrom_dvdram_open_write(cdi);
 	else if (CDROM_CAN(CDC_RAM) &&
 		 !CDROM_CAN(CDC_CD_R|CDC_CD_RW|CDC_DVD|CDC_DVD_R|CDC_MRW|CDC_MO_DRIVE))
 		ret = cdrom_ram_open_write(cdi);
	else if (CDROM_CAN(CDC_MO_DRIVE))
		ret = mo_open_write(cdi);
	else if (!cdrom_is_dvd_rw(cdi))
		ret = 0;

	return ret;
}
