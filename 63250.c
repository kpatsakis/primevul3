static int hub_resume(struct usb_interface *intf)
{
	struct usb_hub *hub = usb_get_intfdata(intf);

	dev_dbg(&intf->dev, "%s\n", __func__);
	hub_activate(hub, HUB_RESUME);

	/*
	 * This should be called only for system resume, not runtime resume.
	 * We can't tell the difference here, so some wakeup requests will be
	 * reported at the wrong time or more than once.  This shouldn't
	 * matter much, so long as they do get reported.
	 */
	report_wakeup_requests(hub);
	return 0;
}
