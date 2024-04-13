static void __exit ib_uverbs_cleanup(void)
{
	ib_unregister_client(&uverbs_client);
	class_destroy(uverbs_class);
	unregister_chrdev_region(IB_UVERBS_BASE_DEV,
				 IB_UVERBS_NUM_FIXED_MINOR);
	unregister_chrdev_region(dynamic_uverbs_dev,
				 IB_UVERBS_NUM_DYNAMIC_MINOR);
}
