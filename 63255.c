static int port_speed_is_ssp(struct usb_device *hdev, int speed_id)
{
	int ssa_count;
	u32 ss_attr;
	int i;
	struct usb_ssp_cap_descriptor *ssp_cap = hdev->bos->ssp_cap;

	if (!ssp_cap)
		return 0;

	ssa_count = le32_to_cpu(ssp_cap->bmAttributes) &
		USB_SSP_SUBLINK_SPEED_ATTRIBS;

	for (i = 0; i <= ssa_count; i++) {
		ss_attr = le32_to_cpu(ssp_cap->bmSublinkSpeedAttr[i]);
		if (speed_id == (ss_attr & USB_SSP_SUBLINK_SPEED_SSID))
			return !!(ss_attr & USB_SSP_SUBLINK_SPEED_LP);
	}
	return 0;
}
