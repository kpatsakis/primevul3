static int cdrom_get_next_writable(struct cdrom_device_info *cdi,
				   long *next_writable)
{
	disc_information di;
	track_information ti;
	__u16 last_track;
	int ret, ti_size;

	if (!CDROM_CAN(CDC_GENERIC_PACKET))
		goto use_last_written;

	ret = cdrom_get_disc_info(cdi, &di);
	if (ret < 0 || ret < offsetof(typeof(di), last_track_lsb)
				+ sizeof(di.last_track_lsb))
		goto use_last_written;

	/* if unit didn't return msb, it's zeroed by cdrom_get_disc_info */
	last_track = (di.last_track_msb << 8) | di.last_track_lsb;
	ti_size = cdrom_get_track_info(cdi, last_track, 1, &ti);
	if (ti_size < 0 || ti_size < offsetof(typeof(ti), track_start))
		goto use_last_written;

	/* if this track is blank, try the previous. */
	if (ti.blank) {
		if (last_track == 1)
			goto use_last_written;
		last_track--;
		ti_size = cdrom_get_track_info(cdi, last_track, 1, &ti);
		if (ti_size < 0)
			goto use_last_written;
	}

	/* if next recordable address field is valid, use it. */
	if (ti.nwa_v && ti_size >= offsetof(typeof(ti), next_writable)
				+ sizeof(ti.next_writable)) {
		*next_writable = be32_to_cpu(ti.next_writable);
		return 0;
	}

use_last_written:
	ret = cdrom_get_last_written(cdi, next_writable);
	if (ret) {
		*next_writable = 0;
		return ret;
	} else {
		*next_writable += 7;
		return 0;
	}
}
