static int cdrom_read_cdda(struct cdrom_device_info *cdi, __u8 __user *ubuf,
			   int lba, int nframes)
{
	int ret;

	if (cdi->cdda_method == CDDA_OLD)
		return cdrom_read_cdda_old(cdi, ubuf, lba, nframes);

retry:
	/*
	 * for anything else than success and io error, we need to retry
	 */
	ret = cdrom_read_cdda_bpc(cdi, ubuf, lba, nframes);
	if (!ret || ret != -EIO)
		return ret;

	/*
	 * I've seen drives get sense 4/8/3 udma crc errors on multi
	 * frame dma, so drop to single frame dma if we need to
	 */
	if (cdi->cdda_method == CDDA_BPC_FULL && nframes > 1) {
		pr_info("dropping to single frame dma\n");
		cdi->cdda_method = CDDA_BPC_SINGLE;
		goto retry;
	}

	/*
	 * so we have an io error of some sort with multi frame dma. if the
	 * condition wasn't a hardware error
	 * problems, not for any error
	 */
	if (cdi->last_sense != 0x04 && cdi->last_sense != 0x0b)
		return ret;

	pr_info("dropping to old style cdda (sense=%x)\n", cdi->last_sense);
	cdi->cdda_method = CDDA_OLD;
	return cdrom_read_cdda_old(cdi, ubuf, lba, nframes);	
}
