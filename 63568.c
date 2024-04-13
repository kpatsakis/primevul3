static int cdrom_read_cd(struct cdrom_device_info *cdi,
			 struct packet_command *cgc, int lba,
			 int blocksize, int nblocks)
{
	const struct cdrom_device_ops *cdo = cdi->ops;

	memset(&cgc->cmd, 0, sizeof(cgc->cmd));
	cgc->cmd[0] = GPCMD_READ_10;
	cgc->cmd[2] = (lba >> 24) & 0xff;
	cgc->cmd[3] = (lba >> 16) & 0xff;
	cgc->cmd[4] = (lba >>  8) & 0xff;
	cgc->cmd[5] = lba & 0xff;
	cgc->cmd[6] = (nblocks >> 16) & 0xff;
	cgc->cmd[7] = (nblocks >>  8) & 0xff;
	cgc->cmd[8] = nblocks & 0xff;
	cgc->buflen = blocksize * nblocks;
	return cdo->generic_packet(cdi, cgc);
}
