struct usb_interface *usb_find_interface(struct usb_driver *drv, int minor)
{
	struct find_interface_arg argb;
	struct device *dev;

	argb.minor = minor;
	argb.drv = &drv->drvwrap.driver;

	dev = bus_find_device(&usb_bus_type, NULL, &argb, __find_interface);

	/* Drop reference count from bus_find_device */
	put_device(dev);

	return dev ? to_usb_interface(dev) : NULL;
}
