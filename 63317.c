static int hwahc_op_start(struct usb_hcd *usb_hcd)
{
	u8 addr;
	int result;
	struct wusbhc *wusbhc = usb_hcd_to_wusbhc(usb_hcd);
	struct hwahc *hwahc = container_of(wusbhc, struct hwahc, wusbhc);

	result = -ENOSPC;
	mutex_lock(&wusbhc->mutex);
	addr = wusb_cluster_id_get();
	if (addr == 0)
		goto error_cluster_id_get;
	result = __hwahc_set_cluster_id(hwahc, addr);
	if (result < 0)
		goto error_set_cluster_id;

	usb_hcd->uses_new_polling = 1;
	set_bit(HCD_FLAG_POLL_RH, &usb_hcd->flags);
	usb_hcd->state = HC_STATE_RUNNING;

	/*
	 * prevent USB core from suspending the root hub since
	 * bus_suspend and bus_resume are not yet supported.
	 */
	pm_runtime_get_noresume(&usb_hcd->self.root_hub->dev);

	result = 0;
out:
	mutex_unlock(&wusbhc->mutex);
	return result;

error_set_cluster_id:
	wusb_cluster_id_put(wusbhc->cluster_id);
error_cluster_id_get:
	goto out;

}
