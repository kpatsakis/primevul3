static int __hwahc_op_set_ptk(struct wusbhc *wusbhc, u8 port_idx, u32 tkid,
			      const void *key, size_t key_size)
{
	int result = -ENOMEM;
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);
	struct wahc *wa = &hwahc->wa;
	u8 iface_no = wa->usb_iface->cur_altsetting->desc.bInterfaceNumber;
	u8 encryption_value;

	/* Tell the host which key to use to talk to the device */
	if (key) {
		u8 key_idx = wusb_key_index(0, WUSB_KEY_INDEX_TYPE_PTK,
					    WUSB_KEY_INDEX_ORIGINATOR_HOST);

		result = __hwahc_dev_set_key(wusbhc, port_idx, tkid,
					     key, key_size, key_idx);
		if (result < 0)
			goto error_set_key;
		encryption_value = wusbhc->ccm1_etd->bEncryptionValue;
	} else {
		/* FIXME: this should come from wusbhc->etd[UNSECURE].value */
		encryption_value = 0;
	}

	/* Set the encryption type for communicating with the device */
	result = usb_control_msg(wa->usb_dev, usb_sndctrlpipe(wa->usb_dev, 0),
			USB_REQ_SET_ENCRYPTION,
			USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE,
			encryption_value, port_idx << 8 | iface_no,
			NULL, 0, USB_CTRL_SET_TIMEOUT);
	if (result < 0)
		dev_err(wusbhc->dev, "Can't set host's WUSB encryption for "
			"port index %u to %s (value %d): %d\n", port_idx,
			wusb_et_name(wusbhc->ccm1_etd->bEncryptionType),
			wusbhc->ccm1_etd->bEncryptionValue, result);
error_set_key:
	return result;
}
