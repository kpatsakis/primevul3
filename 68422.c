static int loop_lookup(struct loop_device **l, int i)
{
	struct loop_device *lo;
	int ret = -ENODEV;

	if (i < 0) {
		int err;

		err = idr_for_each(&loop_index_idr, &find_free_cb, &lo);
		if (err == 1) {
			*l = lo;
			ret = lo->lo_number;
		}
		goto out;
	}

	/* lookup and return a specific i */
	lo = idr_find(&loop_index_idr, i);
	if (lo) {
		*l = lo;
		ret = lo->lo_number;
	}
out:
	return ret;
}
