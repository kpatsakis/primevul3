static int ib_uverbs_create_uapi(struct ib_device *device,
				 struct ib_uverbs_device *uverbs_dev)
{
	struct uverbs_api *uapi;

	uapi = uverbs_alloc_api(device);
	if (IS_ERR(uapi))
		return PTR_ERR(uapi);

	uverbs_dev->uapi = uapi;
	return 0;
}
