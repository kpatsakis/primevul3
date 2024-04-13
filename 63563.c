int cdrom_open(struct cdrom_device_info *cdi, struct block_device *bdev,
	       fmode_t mode)
{
	int ret;

	cd_dbg(CD_OPEN, "entering cdrom_open\n");

	/* if this was a O_NONBLOCK open and we should honor the flags,
	 * do a quick open without drive/disc integrity checks. */
	cdi->use_count++;
	if ((mode & FMODE_NDELAY) && (cdi->options & CDO_USE_FFLAGS)) {
		ret = cdi->ops->open(cdi, 1);
	} else {
		ret = open_for_data(cdi);
		if (ret)
			goto err;
		cdrom_mmc3_profile(cdi);
		if (mode & FMODE_WRITE) {
			ret = -EROFS;
			if (cdrom_open_write(cdi))
				goto err_release;
			if (!CDROM_CAN(CDC_RAM))
				goto err_release;
			ret = 0;
			cdi->media_written = 0;
		}
	}

	if (ret)
		goto err;

	cd_dbg(CD_OPEN, "Use count for \"/dev/%s\" now %d\n",
	       cdi->name, cdi->use_count);
	return 0;
err_release:
	if (CDROM_CAN(CDC_LOCK) && cdi->options & CDO_LOCK) {
		cdi->ops->lock_door(cdi, 0);
		cd_dbg(CD_OPEN, "door unlocked\n");
	}
	cdi->ops->release(cdi);
err:
	cdi->use_count--;
	return ret;
}
