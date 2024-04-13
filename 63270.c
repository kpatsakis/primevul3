static int __find_interface(struct device *dev, void *data)
{
	struct find_interface_arg *arg = data;
	struct usb_interface *intf;

	if (!is_usb_interface(dev))
		return 0;

	if (dev->driver != arg->drv)
		return 0;
	intf = to_usb_interface(dev);
	return intf->minor == arg->minor;
}
