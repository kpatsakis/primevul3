struct usb_device *usb_get_dev(struct usb_device *dev)
{
	if (dev)
		get_device(&dev->dev);
	return dev;
}
