static int loop_sysfs_init(struct loop_device *lo)
{
	return sysfs_create_group(&disk_to_dev(lo->lo_disk)->kobj,
				  &loop_attribute_group);
}
