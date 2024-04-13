void usb_put_intf(struct usb_interface *intf)
{
	if (intf)
		put_device(&intf->dev);
}
