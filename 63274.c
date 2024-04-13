static unsigned usb_bus_is_wusb(struct usb_bus *bus)
{
	struct usb_hcd *hcd = bus_to_hcd(bus);
	return hcd->wireless;
}
