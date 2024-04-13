static void __hwahc_op_wusbhc_stop(struct wusbhc *wusbhc, int delay)
{
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);
	struct wahc *wa = &hwahc->wa;
	u8 iface_no = wa->usb_iface->cur_altsetting->desc.bInterfaceNumber;
	int ret;

	ret = usb_control_msg(wa->usb_dev, usb_sndctrlpipe(wa->usb_dev, 0),
			      WUSB_REQ_CHAN_STOP,
			      USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE,
			      delay * 1000,
			      iface_no,
			      NULL, 0, USB_CTRL_SET_TIMEOUT);
	if (ret == 0)
		msleep(delay);

	wa_nep_disarm(&hwahc->wa);
	__wa_stop(&hwahc->wa);
}
