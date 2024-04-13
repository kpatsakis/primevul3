static int sr_play_trkind(struct cdrom_device_info *cdi,
		struct cdrom_ti *ti)

{
	struct scsi_cd *cd = cdi->handle;
	struct packet_command cgc;
	int result;

	memset(&cgc, 0, sizeof(struct packet_command));
	cgc.timeout = IOCTL_TIMEOUT;
	cgc.cmd[0] = GPCMD_PLAYAUDIO_TI;
	cgc.cmd[4] = ti->cdti_trk0;
	cgc.cmd[5] = ti->cdti_ind0;
	cgc.cmd[7] = ti->cdti_trk1;
	cgc.cmd[8] = ti->cdti_ind1;
	cgc.data_direction = DMA_NONE;

	result = sr_do_ioctl(cd, &cgc);
	if (result == -EDRIVE_CANT_DO_THIS)
		result = sr_fake_playtrkind(cdi, ti);

	return result;
}
