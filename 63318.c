static void hwahc_op_stop(struct usb_hcd *usb_hcd)
{
	struct wusbhc *wusbhc = usb_hcd_to_wusbhc(usb_hcd);

	mutex_lock(&wusbhc->mutex);
	wusb_cluster_id_put(wusbhc->cluster_id);
	mutex_unlock(&wusbhc->mutex);
}
