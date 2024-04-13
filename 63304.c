static int __hwahc_op_dev_info_set(struct wusbhc *wusbhc,
				   struct wusb_dev *wusb_dev)
{
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);
	struct wahc *wa = &hwahc->wa;
	u8 iface_no = wa->usb_iface->cur_altsetting->desc.bInterfaceNumber;
	struct hwa_dev_info *dev_info;
	int ret;

	/* fill out the Device Info buffer and send it */
	dev_info = kzalloc(sizeof(struct hwa_dev_info), GFP_KERNEL);
	if (!dev_info)
		return -ENOMEM;
	uwb_mas_bm_copy_le(dev_info->bmDeviceAvailability,
			   &wusb_dev->availability);
	dev_info->bDeviceAddress = wusb_dev->addr;

	/*
	 * If the descriptors haven't been read yet, use a default PHY
	 * rate of 53.3 Mbit/s only.  The correct value will be used
	 * when this will be called again as part of the
	 * authentication process (which occurs after the descriptors
	 * have been read).
	 */
	if (wusb_dev->wusb_cap_descr)
		dev_info->wPHYRates = wusb_dev->wusb_cap_descr->wPHYRates;
	else
		dev_info->wPHYRates = cpu_to_le16(USB_WIRELESS_PHY_53);

	ret = usb_control_msg(wa->usb_dev, usb_sndctrlpipe(wa->usb_dev, 0),
			WUSB_REQ_SET_DEV_INFO,
			USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE,
			0, wusb_dev->port_idx << 8 | iface_no,
			dev_info, sizeof(struct hwa_dev_info),
			USB_CTRL_SET_TIMEOUT);
	kfree(dev_info);
	return ret;
}
