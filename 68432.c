loop_set_status(struct loop_device *lo, const struct loop_info64 *info)
{
	int err;
	struct loop_func_table *xfer;
	kuid_t uid = current_uid();

	if (lo->lo_encrypt_key_size &&
	    !uid_eq(lo->lo_key_owner, uid) &&
	    !capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (lo->lo_state != Lo_bound)
		return -ENXIO;
	if ((unsigned int) info->lo_encrypt_key_size > LO_KEY_SIZE)
		return -EINVAL;

	/* I/O need to be drained during transfer transition */
	blk_mq_freeze_queue(lo->lo_queue);

	err = loop_release_xfer(lo);
	if (err)
		goto exit;

	if (info->lo_encrypt_type) {
		unsigned int type = info->lo_encrypt_type;

		if (type >= MAX_LO_CRYPT)
			return -EINVAL;
		xfer = xfer_funcs[type];
		if (xfer == NULL)
			return -EINVAL;
	} else
		xfer = NULL;

	err = loop_init_xfer(lo, xfer, info);
	if (err)
		goto exit;

	if (lo->lo_offset != info->lo_offset ||
	    lo->lo_sizelimit != info->lo_sizelimit) {
		if (figure_loop_size(lo, info->lo_offset, info->lo_sizelimit)) {
			err = -EFBIG;
			goto exit;
		}
	}

	loop_config_discard(lo);

	memcpy(lo->lo_file_name, info->lo_file_name, LO_NAME_SIZE);
	memcpy(lo->lo_crypt_name, info->lo_crypt_name, LO_NAME_SIZE);
	lo->lo_file_name[LO_NAME_SIZE-1] = 0;
	lo->lo_crypt_name[LO_NAME_SIZE-1] = 0;

	if (!xfer)
		xfer = &none_funcs;
	lo->transfer = xfer->transfer;
	lo->ioctl = xfer->ioctl;

	if ((lo->lo_flags & LO_FLAGS_AUTOCLEAR) !=
	     (info->lo_flags & LO_FLAGS_AUTOCLEAR))
		lo->lo_flags ^= LO_FLAGS_AUTOCLEAR;

	lo->lo_encrypt_key_size = info->lo_encrypt_key_size;
	lo->lo_init[0] = info->lo_init[0];
	lo->lo_init[1] = info->lo_init[1];
	if (info->lo_encrypt_key_size) {
		memcpy(lo->lo_encrypt_key, info->lo_encrypt_key,
		       info->lo_encrypt_key_size);
		lo->lo_key_owner = uid;
	}

	/* update dio if lo_offset or transfer is changed */
	__loop_update_dio(lo, lo->use_dio);

 exit:
	blk_mq_unfreeze_queue(lo->lo_queue);

	if (!err && (info->lo_flags & LO_FLAGS_PARTSCAN) &&
	     !(lo->lo_flags & LO_FLAGS_PARTSCAN)) {
		lo->lo_flags |= LO_FLAGS_PARTSCAN;
		lo->lo_disk->flags &= ~GENHD_FL_NO_PART_SCAN;
		loop_reread_partitions(lo, lo->lo_device);
	}

	return err;
}
