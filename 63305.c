static int __hwahc_op_mmcie_add(struct wusbhc *wusbhc, u8 interval,
				u8 repeat_cnt, u8 handle,
				struct wuie_hdr *wuie)
{
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);
	struct wahc *wa = &hwahc->wa;
	u8 iface_no = wa->usb_iface->cur_altsetting->desc.bInterfaceNumber;

	return usb_control_msg(wa->usb_dev, usb_sndctrlpipe(wa->usb_dev, 0),
			WUSB_REQ_ADD_MMC_IE,
			USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE,
			interval << 8 | repeat_cnt,
			handle << 8 | iface_no,
			wuie, wuie->bLength, USB_CTRL_SET_TIMEOUT);
}
