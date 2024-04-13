void usb_put_dev(struct usb_device *dev)
{
	if (dev)
		put_device(&dev->dev);
}
