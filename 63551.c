static int cdrom_is_dvd_rw(struct cdrom_device_info *cdi)
{
	switch (cdi->mmc3_profile) {
	case 0x12:	/* DVD-RAM	*/
	case 0x1A:	/* DVD+RW	*/
	case 0x43:	/* BD-RE	*/
		return 0;
	default:
		return 1;
	}
}
