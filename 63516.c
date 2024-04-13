static void cdrom_count_tracks(struct cdrom_device_info *cdi, tracktype *tracks)
{
	struct cdrom_tochdr header;
	struct cdrom_tocentry entry;
	int ret, i;
	tracks->data = 0;
	tracks->audio = 0;
	tracks->cdi = 0;
	tracks->xa = 0;
	tracks->error = 0;
	cd_dbg(CD_COUNT_TRACKS, "entering cdrom_count_tracks\n");
	/* Grab the TOC header so we can see how many tracks there are */
	ret = cdi->ops->audio_ioctl(cdi, CDROMREADTOCHDR, &header);
	if (ret) {
		if (ret == -ENOMEDIUM)
			tracks->error = CDS_NO_DISC;
		else
			tracks->error = CDS_NO_INFO;
		return;
	}
	/* check what type of tracks are on this disc */
	entry.cdte_format = CDROM_MSF;
	for (i = header.cdth_trk0; i <= header.cdth_trk1; i++) {
		entry.cdte_track = i;
		if (cdi->ops->audio_ioctl(cdi, CDROMREADTOCENTRY, &entry)) {
			tracks->error = CDS_NO_INFO;
			return;
		}
		if (entry.cdte_ctrl & CDROM_DATA_TRACK) {
			if (entry.cdte_format == 0x10)
				tracks->cdi++;
			else if (entry.cdte_format == 0x20)
				tracks->xa++;
			else
				tracks->data++;
		} else {
			tracks->audio++;
		}
		cd_dbg(CD_COUNT_TRACKS, "track %d: format=%d, ctrl=%d\n",
		       i, entry.cdte_format, entry.cdte_ctrl);
	}
	cd_dbg(CD_COUNT_TRACKS, "disc has %d tracks: %d=audio %d=data %d=Cd-I %d=XA\n",
	       header.cdth_trk1, tracks->audio, tracks->data,
	       tracks->cdi, tracks->xa);
}
