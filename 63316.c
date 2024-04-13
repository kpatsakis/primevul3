static int hwahc_op_reset(struct usb_hcd *usb_hcd)
{
	int result;
	struct wusbhc *wusbhc = usb_hcd_to_wusbhc(usb_hcd);
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);
	struct device *dev = &hwahc->wa.usb_iface->dev;

	mutex_lock(&wusbhc->mutex);
	wa_nep_disarm(&hwahc->wa);
	result = __wa_set_feature(&hwahc->wa, WA_RESET);
	if (result < 0) {
		dev_err(dev, "error commanding HC to reset: %d\n", result);
		goto error_unlock;
	}
	result = __wa_wait_status(&hwahc->wa, WA_STATUS_RESETTING, 0);
	if (result < 0) {
		dev_err(dev, "error waiting for HC to reset: %d\n", result);
		goto error_unlock;
	}
error_unlock:
	mutex_unlock(&wusbhc->mutex);
	return result;
}
