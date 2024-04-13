int usb_for_each_dev(void *data, int (*fn)(struct usb_device *, void *))
{
	struct each_dev_arg arg = {data, fn};

	return bus_for_each_dev(&usb_bus_type, NULL, &arg, __each_dev);
}
