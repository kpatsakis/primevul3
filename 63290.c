int usb_find_common_endpoints_reverse(struct usb_host_interface *alt,
		struct usb_endpoint_descriptor **bulk_in,
		struct usb_endpoint_descriptor **bulk_out,
		struct usb_endpoint_descriptor **int_in,
		struct usb_endpoint_descriptor **int_out)
{
	struct usb_endpoint_descriptor *epd;
	int i;

	if (bulk_in)
		*bulk_in = NULL;
	if (bulk_out)
		*bulk_out = NULL;
	if (int_in)
		*int_in = NULL;
	if (int_out)
		*int_out = NULL;

	for (i = alt->desc.bNumEndpoints - 1; i >= 0; --i) {
		epd = &alt->endpoint[i].desc;

		if (match_endpoint(epd, bulk_in, bulk_out, int_in, int_out))
			return 0;
	}

	return -ENXIO;
}
