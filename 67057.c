int sr_is_xa(Scsi_CD *cd)
{
	unsigned char *raw_sector;
	int is_xa;

	if (!xa_test)
		return 0;

	raw_sector = kmalloc(2048, GFP_KERNEL | SR_GFP_DMA(cd));
	if (!raw_sector)
		return -ENOMEM;
	if (0 == sr_read_sector(cd, cd->ms_offset + 16,
				CD_FRAMESIZE_RAW1, raw_sector)) {
		is_xa = (raw_sector[3] == 0x02) ? 1 : 0;
	} else {
		/* read a raw sector failed for some reason. */
		is_xa = -1;
	}
	kfree(raw_sector);
#ifdef DEBUG
	sr_printk(KERN_INFO, cd, "sr_is_xa: %d\n", is_xa);
#endif
	return is_xa;
}
