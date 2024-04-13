static int __hwahc_dev_set_key(struct wusbhc *wusbhc, u8 port_idx, u32 tkid,
			       const void *key, size_t key_size,
			       u8 key_idx)
{
	int result = -ENOMEM;
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);
	struct wahc *wa = &hwahc->wa;
	u8 iface_no = wa->usb_iface->cur_altsetting->desc.bInterfaceNumber;
	struct usb_key_descriptor *keyd;
	size_t keyd_len;

	keyd_len = sizeof(*keyd) + key_size;
	keyd = kzalloc(keyd_len, GFP_KERNEL);
	if (keyd == NULL)
		return -ENOMEM;

	keyd->bLength = keyd_len;
	keyd->bDescriptorType = USB_DT_KEY;
	keyd->tTKID[0] = (tkid >>  0) & 0xff;
	keyd->tTKID[1] = (tkid >>  8) & 0xff;
	keyd->tTKID[2] = (tkid >> 16) & 0xff;
	memcpy(keyd->bKeyData, key, key_size);

	result = usb_control_msg(wa->usb_dev, usb_sndctrlpipe(wa->usb_dev, 0),
			USB_REQ_SET_DESCRIPTOR,
			USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE,
			USB_DT_KEY << 8 | key_idx,
			port_idx << 8 | iface_no,
			keyd, keyd_len, USB_CTRL_SET_TIMEOUT);

	kzfree(keyd); /* clear keys etc. */
	return result;
}
