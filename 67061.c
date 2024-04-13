static int sr_read_sector(Scsi_CD *cd, int lba, int blksize, unsigned char *dest)
{
	struct packet_command cgc;
	int rc;

	/* we try the READ CD command first... */
	if (cd->readcd_known) {
		rc = sr_read_cd(cd, dest, lba, 0, blksize);
		if (-EDRIVE_CANT_DO_THIS != rc)
			return rc;
		cd->readcd_known = 0;
		sr_printk(KERN_INFO, cd,
			  "CDROM does'nt support READ CD (0xbe) command\n");
		/* fall & retry the other way */
	}
	/* ... if this fails, we switch the blocksize using MODE SELECT */
	if (blksize != cd->device->sector_size) {
		if (0 != (rc = sr_set_blocklength(cd, blksize)))
			return rc;
	}
#ifdef DEBUG
	sr_printk(KERN_INFO, cd, "sr_read_sector lba=%d blksize=%d\n",
		  lba, blksize);
#endif

	memset(&cgc, 0, sizeof(struct packet_command));
	cgc.cmd[0] = GPCMD_READ_10;
	cgc.cmd[2] = (unsigned char) (lba >> 24) & 0xff;
	cgc.cmd[3] = (unsigned char) (lba >> 16) & 0xff;
	cgc.cmd[4] = (unsigned char) (lba >> 8) & 0xff;
	cgc.cmd[5] = (unsigned char) lba & 0xff;
	cgc.cmd[8] = 1;
	cgc.buffer = dest;
	cgc.buflen = blksize;
	cgc.data_direction = DMA_FROM_DEVICE;
	cgc.timeout = IOCTL_TIMEOUT;
	rc = sr_do_ioctl(cd, &cgc);

	return rc;
}
