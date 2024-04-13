static void hwahc_destroy(struct hwahc *hwahc)
{
	struct wusbhc *wusbhc = &hwahc->wusbhc;

	mutex_lock(&wusbhc->mutex);
	__wa_destroy(&hwahc->wa);
	wusbhc_destroy(&hwahc->wusbhc);
	hwahc_security_release(hwahc);
	hwahc->wusbhc.dev = NULL;
	uwb_rc_put(wusbhc->uwb_rc);
	usb_put_intf(hwahc->wa.usb_iface);
	usb_put_dev(hwahc->wa.usb_dev);
	mutex_unlock(&wusbhc->mutex);
}
