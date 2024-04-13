static int hwahc_op_urb_dequeue(struct usb_hcd *usb_hcd, struct urb *urb,
				int status)
{
	struct wusbhc *wusbhc = usb_hcd_to_wusbhc(usb_hcd);
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);

	return wa_urb_dequeue(&hwahc->wa, urb, status);
}
