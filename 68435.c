static void loop_sysfs_exit(struct loop_device *lo)
{
	sysfs_remove_group(&disk_to_dev(lo->lo_disk)->kobj,
			   &loop_attribute_group);
}
