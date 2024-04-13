static int sr_fake_playtrkind(struct cdrom_device_info *cdi, struct cdrom_ti *ti)
{
	struct cdrom_tocentry trk0_te, trk1_te;
	struct cdrom_tochdr tochdr;
	struct packet_command cgc;
	int ntracks, ret;

	ret = sr_read_tochdr(cdi, &tochdr);
	if (ret)
		return ret;

	ntracks = tochdr.cdth_trk1 - tochdr.cdth_trk0 + 1;
	
	if (ti->cdti_trk1 == ntracks) 
		ti->cdti_trk1 = CDROM_LEADOUT;
	else if (ti->cdti_trk1 != CDROM_LEADOUT)
		ti->cdti_trk1 ++;

	trk0_te.cdte_track = ti->cdti_trk0;
	trk0_te.cdte_format = CDROM_MSF;
	trk1_te.cdte_track = ti->cdti_trk1;
	trk1_te.cdte_format = CDROM_MSF;
	
	ret = sr_read_tocentry(cdi, &trk0_te);
	if (ret)
		return ret;
	ret = sr_read_tocentry(cdi, &trk1_te);
	if (ret)
		return ret;

	memset(&cgc, 0, sizeof(struct packet_command));
	cgc.cmd[0] = GPCMD_PLAY_AUDIO_MSF;
	cgc.cmd[3] = trk0_te.cdte_addr.msf.minute;
	cgc.cmd[4] = trk0_te.cdte_addr.msf.second;
	cgc.cmd[5] = trk0_te.cdte_addr.msf.frame;
	cgc.cmd[6] = trk1_te.cdte_addr.msf.minute;
	cgc.cmd[7] = trk1_te.cdte_addr.msf.second;
	cgc.cmd[8] = trk1_te.cdte_addr.msf.frame;
	cgc.data_direction = DMA_NONE;
	cgc.timeout = IOCTL_TIMEOUT;
	return sr_do_ioctl(cdi->handle, &cgc);
}
