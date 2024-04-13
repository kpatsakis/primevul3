static int hwahc_op_urb_enqueue(struct usb_hcd *usb_hcd, struct urb *urb,
				gfp_t gfp)
{
	struct wusbhc *wusbhc = usb_hcd_to_wusbhc(usb_hcd);
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);

	return wa_urb_enqueue(&hwahc->wa, urb->ep, urb, gfp);
}
