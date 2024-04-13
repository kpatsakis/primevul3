static unsigned hub_is_wusb(struct usb_hub *hub)
{
	struct usb_hcd *hcd;
	if (hub->hdev->parent != NULL)  /* not a root hub? */
		return 0;
	hcd = bus_to_hcd(hub->hdev->bus);
	return hcd->wireless;
}
