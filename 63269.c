static int __each_dev(struct device *dev, void *data)
{
	struct each_dev_arg *arg = (struct each_dev_arg *)data;

	/* There are struct usb_interface on the same bus, filter them out */
	if (!is_usb_device(dev))
		return 0;

	return arg->fn(to_usb_device(dev), arg->data);
}
