static bool hub_descriptor_is_sane(struct usb_host_interface *desc)
{
	/* Some hubs have a subclass of 1, which AFAICT according to the */
	/*  specs is not defined, but it works */
	if (desc->desc.bInterfaceSubClass != 0 &&
	    desc->desc.bInterfaceSubClass != 1)
		return false;

	/* Multiple endpoints? What kind of mutant ninja-hub is this? */
	if (desc->desc.bNumEndpoints != 1)
		return false;

	/* If the first endpoint is not interrupt IN, we'd better punt! */
	if (!usb_endpoint_is_int_in(&desc->endpoint[0].desc))
		return false;

        return true;
}
