static void ib_uverbs_release_dev(struct device *device)
{
	struct ib_uverbs_device *dev =
			container_of(device, struct ib_uverbs_device, dev);

	uverbs_destroy_api(dev->uapi);
	cleanup_srcu_struct(&dev->disassociate_srcu);
	kfree(dev);
}
