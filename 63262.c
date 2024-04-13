void usb_enable_lpm(struct usb_device *udev)
{
	struct usb_hcd *hcd;
	struct usb_hub *hub;
	struct usb_port *port_dev;

	if (!udev || !udev->parent ||
			udev->speed < USB_SPEED_SUPER ||
			!udev->lpm_capable ||
			udev->state < USB_STATE_DEFAULT)
		return;

	udev->lpm_disable_count--;
	hcd = bus_to_hcd(udev->bus);
	/* Double check that we can both enable and disable LPM.
	 * Device must be configured to accept set feature U1/U2 timeout.
	 */
	if (!hcd || !hcd->driver->enable_usb3_lpm_timeout ||
			!hcd->driver->disable_usb3_lpm_timeout)
		return;

	if (udev->lpm_disable_count > 0)
		return;

	hub = usb_hub_to_struct_hub(udev->parent);
	if (!hub)
		return;

	port_dev = hub->ports[udev->portnum - 1];

	if (port_dev->usb3_lpm_u1_permit)
		usb_enable_link_state(hcd, udev, USB3_LPM_U1);

	if (port_dev->usb3_lpm_u2_permit)
		usb_enable_link_state(hcd, udev, USB3_LPM_U2);
}
