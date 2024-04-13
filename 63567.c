static int cdrom_read_block(struct cdrom_device_info *cdi,
			    struct packet_command *cgc,
			    int lba, int nblocks, int format, int blksize)
{
	const struct cdrom_device_ops *cdo = cdi->ops;

	memset(&cgc->cmd, 0, sizeof(cgc->cmd));
	cgc->cmd[0] = GPCMD_READ_CD;
	/* expected sector size - cdda,mode1,etc. */
	cgc->cmd[1] = format << 2;
	/* starting address */
	cgc->cmd[2] = (lba >> 24) & 0xff;
	cgc->cmd[3] = (lba >> 16) & 0xff;
	cgc->cmd[4] = (lba >>  8) & 0xff;
	cgc->cmd[5] = lba & 0xff;
	/* number of blocks */
	cgc->cmd[6] = (nblocks >> 16) & 0xff;
	cgc->cmd[7] = (nblocks >>  8) & 0xff;
	cgc->cmd[8] = nblocks & 0xff;
	cgc->buflen = blksize * nblocks;
	
	/* set the header info returned */
	switch (blksize) {
	case CD_FRAMESIZE_RAW0	: cgc->cmd[9] = 0x58; break;
	case CD_FRAMESIZE_RAW1	: cgc->cmd[9] = 0x78; break;
	case CD_FRAMESIZE_RAW	: cgc->cmd[9] = 0xf8; break;
	default			: cgc->cmd[9] = 0x10;
	}
	
	return cdo->generic_packet(cdi, cgc);
}
