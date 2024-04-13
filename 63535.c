static int cdrom_ioctl_disc_status(struct cdrom_device_info *cdi)
{
	tracktype tracks;

	cd_dbg(CD_DO_IOCTL, "entering CDROM_DISC_STATUS\n");

	cdrom_count_tracks(cdi, &tracks);
	if (tracks.error)
		return tracks.error;

	/* Policy mode on */
	if (tracks.audio > 0) {
		if (!tracks.data && !tracks.cdi && !tracks.xa)
			return CDS_AUDIO;
		else
			return CDS_MIXED;
	}

	if (tracks.cdi > 0)
		return CDS_XA_2_2;
	if (tracks.xa > 0)
		return CDS_XA_2_1;
	if (tracks.data > 0)
		return CDS_DATA_1;
	/* Policy mode off */

	cd_dbg(CD_WARNING, "This disc doesn't have any tracks I recognize!\n");
	return CDS_NO_INFO;
}
