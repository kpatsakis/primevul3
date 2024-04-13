static inline bool bio_check_ro(struct bio *bio, struct hd_struct *part)
{
	if (part->policy && op_is_write(bio_op(bio))) {
		char b[BDEVNAME_SIZE];

		printk(KERN_ERR
		       "generic_make_request: Trying to write "
			"to read-only block-device %s (partno %d)\n",
			bio_devname(bio, b), part->partno);
		return true;
	}

	return false;
}
