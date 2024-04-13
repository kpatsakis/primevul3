static int cdrom_media_erasable(struct cdrom_device_info *cdi)
{
	disc_information di;
	int ret;

	ret = cdrom_get_disc_info(cdi, &di);
	if (ret < 0 || ret < offsetof(typeof(di), n_first_track))
		return -1;

	return di.erasable;
}
