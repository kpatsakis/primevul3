static int hwahc_op_get_frame_number(struct usb_hcd *usb_hcd)
{
	struct wusbhc *wusbhc = usb_hcd_to_wusbhc(usb_hcd);
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);
	struct wahc *wa = &hwahc->wa;

	/*
	 * We cannot query the HWA for the WUSB time since that requires sending
	 * a synchronous URB and this function can be called in_interrupt.
	 * Instead, query the USB frame number for our parent and use that.
	 */
	return usb_get_current_frame_number(wa->usb_dev);
}
