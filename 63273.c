struct usb_host_interface *usb_altnum_to_altsetting(
					const struct usb_interface *intf,
					unsigned int altnum)
{
	int i;

	for (i = 0; i < intf->num_altsetting; i++) {
		if (intf->altsetting[i].desc.bAlternateSetting == altnum)
			return &intf->altsetting[i];
	}
	return NULL;
}
