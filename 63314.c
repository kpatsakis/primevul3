static void hwahc_op_endpoint_disable(struct usb_hcd *usb_hcd,
				      struct usb_host_endpoint *ep)
{
	struct wusbhc *wusbhc = usb_hcd_to_wusbhc(usb_hcd);
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);

	rpipe_ep_disable(&hwahc->wa, ep);
}
