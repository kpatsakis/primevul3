int sr_disk_status(struct cdrom_device_info *cdi)
{
	Scsi_CD *cd = cdi->handle;
	struct cdrom_tochdr toc_h;
	struct cdrom_tocentry toc_e;
	int i, rc, have_datatracks = 0;

	/* look for data tracks */
	rc = sr_read_tochdr(cdi, &toc_h);
	if (rc)
		return (rc == -ENOMEDIUM) ? CDS_NO_DISC : CDS_NO_INFO;

	for (i = toc_h.cdth_trk0; i <= toc_h.cdth_trk1; i++) {
		toc_e.cdte_track = i;
		toc_e.cdte_format = CDROM_LBA;
		if (sr_read_tocentry(cdi, &toc_e))
			return CDS_NO_INFO;
		if (toc_e.cdte_ctrl & CDROM_DATA_TRACK) {
			have_datatracks = 1;
			break;
		}
	}
	if (!have_datatracks)
		return CDS_AUDIO;

	if (cd->xa_flag)
		return CDS_XA_2_1;
	else
		return CDS_DATA_1;
}
