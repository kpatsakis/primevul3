static ssize_t loop_attr_show(struct device *dev, char *page,
			      ssize_t (*callback)(struct loop_device *, char *))
{
	struct gendisk *disk = dev_to_disk(dev);
	struct loop_device *lo = disk->private_data;

	return callback(lo, page);
}
