static int hwahc_probe(struct usb_interface *usb_iface,
		       const struct usb_device_id *id)
{
	int result;
	struct usb_hcd *usb_hcd;
	struct wusbhc *wusbhc;
	struct hwahc *hwahc;
	struct device *dev = &usb_iface->dev;

	result = -ENOMEM;
	usb_hcd = usb_create_hcd(&hwahc_hc_driver, &usb_iface->dev, "wusb-hwa");
	if (usb_hcd == NULL) {
		dev_err(dev, "unable to allocate instance\n");
		goto error_alloc;
	}
	usb_hcd->wireless = 1;
	usb_hcd->self.sg_tablesize = ~0;
	wusbhc = usb_hcd_to_wusbhc(usb_hcd);
	hwahc = container_of(wusbhc, struct hwahc, wusbhc);
	hwahc_init(hwahc);
	result = hwahc_create(hwahc, usb_iface, id->driver_info);
	if (result < 0) {
		dev_err(dev, "Cannot initialize internals: %d\n", result);
		goto error_hwahc_create;
	}
	result = usb_add_hcd(usb_hcd, 0, 0);
	if (result < 0) {
		dev_err(dev, "Cannot add HCD: %d\n", result);
		goto error_add_hcd;
	}
	device_wakeup_enable(usb_hcd->self.controller);
	result = wusbhc_b_create(&hwahc->wusbhc);
	if (result < 0) {
		dev_err(dev, "Cannot setup phase B of WUSBHC: %d\n", result);
		goto error_wusbhc_b_create;
	}
	return 0;

error_wusbhc_b_create:
	usb_remove_hcd(usb_hcd);
error_add_hcd:
	hwahc_destroy(hwahc);
error_hwahc_create:
	usb_put_hcd(usb_hcd);
error_alloc:
	return result;
}
