static int cdrom_dvdram_open_write(struct cdrom_device_info *cdi)
{
	int ret = cdrom_media_erasable(cdi);

	/*
	 * allow writable open if media info read worked and media is
	 * erasable, _or_ if it fails since not all drives support it
	 */
	if (!ret)
		return 1;

	return 0;
}
