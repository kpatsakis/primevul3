static int __init loop_init(void)
{
	int i, nr;
	unsigned long range;
	struct loop_device *lo;
	int err;

	part_shift = 0;
	if (max_part > 0) {
		part_shift = fls(max_part);

		/*
		 * Adjust max_part according to part_shift as it is exported
		 * to user space so that user can decide correct minor number
		 * if [s]he want to create more devices.
		 *
		 * Note that -1 is required because partition 0 is reserved
		 * for the whole disk.
		 */
		max_part = (1UL << part_shift) - 1;
	}

	if ((1UL << part_shift) > DISK_MAX_PARTS) {
		err = -EINVAL;
		goto err_out;
	}

	if (max_loop > 1UL << (MINORBITS - part_shift)) {
		err = -EINVAL;
		goto err_out;
	}

	/*
	 * If max_loop is specified, create that many devices upfront.
	 * This also becomes a hard limit. If max_loop is not specified,
	 * create CONFIG_BLK_DEV_LOOP_MIN_COUNT loop devices at module
	 * init time. Loop devices can be requested on-demand with the
	 * /dev/loop-control interface, or be instantiated by accessing
	 * a 'dead' device node.
	 */
	if (max_loop) {
		nr = max_loop;
		range = max_loop << part_shift;
	} else {
		nr = CONFIG_BLK_DEV_LOOP_MIN_COUNT;
		range = 1UL << MINORBITS;
	}

	err = misc_register(&loop_misc);
	if (err < 0)
		goto err_out;


	if (register_blkdev(LOOP_MAJOR, "loop")) {
		err = -EIO;
		goto misc_out;
	}

	blk_register_region(MKDEV(LOOP_MAJOR, 0), range,
				  THIS_MODULE, loop_probe, NULL, NULL);

	/* pre-create number of devices given by config or max_loop */
	mutex_lock(&loop_index_mutex);
	for (i = 0; i < nr; i++)
		loop_add(&lo, i);
	mutex_unlock(&loop_index_mutex);

	printk(KERN_INFO "loop: module loaded\n");
	return 0;

misc_out:
	misc_deregister(&loop_misc);
err_out:
	return err;
}
