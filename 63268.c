static bool use_new_scheme(struct usb_device *udev, int retry,
			   struct usb_port *port_dev)
{
	int old_scheme_first_port =
		port_dev->quirks & USB_PORT_QUIRK_OLD_SCHEME;
	int quick_enumeration = (udev->speed == USB_SPEED_HIGH);

	if (udev->speed >= USB_SPEED_SUPER)
		return false;

	return USE_NEW_SCHEME(retry, old_scheme_first_port || old_scheme_first
			      || quick_enumeration);
}
