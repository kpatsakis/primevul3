int sr_audio_ioctl(struct cdrom_device_info *cdi, unsigned int cmd, void *arg)
{
	switch (cmd) {
	case CDROMREADTOCHDR:
		return sr_read_tochdr(cdi, arg);
	case CDROMREADTOCENTRY:
		return sr_read_tocentry(cdi, arg);
	case CDROMPLAYTRKIND:
		return sr_play_trkind(cdi, arg);
	default:
		return -EINVAL;
	}
}
