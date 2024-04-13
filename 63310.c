static int __hwahc_set_cluster_id(struct hwahc *hwahc, u8 cluster_id)
{
	int result;
	struct wusbhc *wusbhc = &hwahc->wusbhc;
	struct wahc *wa = &hwahc->wa;
	struct device *dev = &wa->usb_iface->dev;

	result = usb_control_msg(wa->usb_dev, usb_sndctrlpipe(wa->usb_dev, 0),
			WUSB_REQ_SET_CLUSTER_ID,
			USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE,
			cluster_id,
			wa->usb_iface->cur_altsetting->desc.bInterfaceNumber,
			NULL, 0, USB_CTRL_SET_TIMEOUT);
	if (result < 0)
		dev_err(dev, "Cannot set WUSB Cluster ID to 0x%02x: %d\n",
			cluster_id, result);
	else
		wusbhc->cluster_id = cluster_id;
	dev_info(dev, "Wireless USB Cluster ID set to 0x%02x\n", cluster_id);
	return result;
}
