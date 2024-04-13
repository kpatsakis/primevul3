static struct kobject *loop_probe(dev_t dev, int *part, void *data)
{
	struct loop_device *lo;
	struct kobject *kobj;
	int err;

	mutex_lock(&loop_index_mutex);
	err = loop_lookup(&lo, MINOR(dev) >> part_shift);
	if (err < 0)
		err = loop_add(&lo, MINOR(dev) >> part_shift);
	if (err < 0)
		kobj = NULL;
	else
		kobj = get_disk(lo->lo_disk);
	mutex_unlock(&loop_index_mutex);

	*part = 0;
	return kobj;
}
