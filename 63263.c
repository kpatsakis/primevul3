int usb_port_disable(struct usb_device *udev)
{
	struct usb_hub *hub = usb_hub_to_struct_hub(udev->parent);

	return hub_port_disable(hub, udev->portnum, 0);
}
