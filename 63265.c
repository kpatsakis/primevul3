static int usb_reset_and_verify_device(struct usb_device *udev)
{
	struct usb_device		*parent_hdev = udev->parent;
	struct usb_hub			*parent_hub;
	struct usb_hcd			*hcd = bus_to_hcd(udev->bus);
	struct usb_device_descriptor	descriptor = udev->descriptor;
	struct usb_host_bos		*bos;
	int				i, j, ret = 0;
	int				port1 = udev->portnum;

	if (udev->state == USB_STATE_NOTATTACHED ||
			udev->state == USB_STATE_SUSPENDED) {
		dev_dbg(&udev->dev, "device reset not allowed in state %d\n",
				udev->state);
		return -EINVAL;
	}

	if (!parent_hdev)
		return -EISDIR;

	parent_hub = usb_hub_to_struct_hub(parent_hdev);

	/* Disable USB2 hardware LPM.
	 * It will be re-enabled by the enumeration process.
	 */
	if (udev->usb2_hw_lpm_enabled == 1)
		usb_set_usb2_hardware_lpm(udev, 0);

	/* Disable LPM while we reset the device and reinstall the alt settings.
	 * Device-initiated LPM, and system exit latency settings are cleared
	 * when the device is reset, so we have to set them up again.
	 */
	ret = usb_unlocked_disable_lpm(udev);
	if (ret) {
		dev_err(&udev->dev, "%s Failed to disable LPM\n", __func__);
		goto re_enumerate_no_bos;
	}

	bos = udev->bos;
	udev->bos = NULL;

	for (i = 0; i < SET_CONFIG_TRIES; ++i) {

		/* ep0 maxpacket size may change; let the HCD know about it.
		 * Other endpoints will be handled by re-enumeration. */
		usb_ep0_reinit(udev);
		ret = hub_port_init(parent_hub, udev, port1, i);
		if (ret >= 0 || ret == -ENOTCONN || ret == -ENODEV)
			break;
	}

	if (ret < 0)
		goto re_enumerate;

	/* Device might have changed firmware (DFU or similar) */
	if (descriptors_changed(udev, &descriptor, bos)) {
		dev_info(&udev->dev, "device firmware changed\n");
		udev->descriptor = descriptor;	/* for disconnect() calls */
		goto re_enumerate;
	}

	/* Restore the device's previous configuration */
	if (!udev->actconfig)
		goto done;

	mutex_lock(hcd->bandwidth_mutex);
	ret = usb_hcd_alloc_bandwidth(udev, udev->actconfig, NULL, NULL);
	if (ret < 0) {
		dev_warn(&udev->dev,
				"Busted HC?  Not enough HCD resources for "
				"old configuration.\n");
		mutex_unlock(hcd->bandwidth_mutex);
		goto re_enumerate;
	}
	ret = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
			USB_REQ_SET_CONFIGURATION, 0,
			udev->actconfig->desc.bConfigurationValue, 0,
			NULL, 0, USB_CTRL_SET_TIMEOUT);
	if (ret < 0) {
		dev_err(&udev->dev,
			"can't restore configuration #%d (error=%d)\n",
			udev->actconfig->desc.bConfigurationValue, ret);
		mutex_unlock(hcd->bandwidth_mutex);
		goto re_enumerate;
	}
	mutex_unlock(hcd->bandwidth_mutex);
	usb_set_device_state(udev, USB_STATE_CONFIGURED);

	/* Put interfaces back into the same altsettings as before.
	 * Don't bother to send the Set-Interface request for interfaces
	 * that were already in altsetting 0; besides being unnecessary,
	 * many devices can't handle it.  Instead just reset the host-side
	 * endpoint state.
	 */
	for (i = 0; i < udev->actconfig->desc.bNumInterfaces; i++) {
		struct usb_host_config *config = udev->actconfig;
		struct usb_interface *intf = config->interface[i];
		struct usb_interface_descriptor *desc;

		desc = &intf->cur_altsetting->desc;
		if (desc->bAlternateSetting == 0) {
			usb_disable_interface(udev, intf, true);
			usb_enable_interface(udev, intf, true);
			ret = 0;
		} else {
			/* Let the bandwidth allocation function know that this
			 * device has been reset, and it will have to use
			 * alternate setting 0 as the current alternate setting.
			 */
			intf->resetting_device = 1;
			ret = usb_set_interface(udev, desc->bInterfaceNumber,
					desc->bAlternateSetting);
			intf->resetting_device = 0;
		}
		if (ret < 0) {
			dev_err(&udev->dev, "failed to restore interface %d "
				"altsetting %d (error=%d)\n",
				desc->bInterfaceNumber,
				desc->bAlternateSetting,
				ret);
			goto re_enumerate;
		}
		/* Resetting also frees any allocated streams */
		for (j = 0; j < intf->cur_altsetting->desc.bNumEndpoints; j++)
			intf->cur_altsetting->endpoint[j].streams = 0;
	}

done:
	/* Now that the alt settings are re-installed, enable LTM and LPM. */
	usb_set_usb2_hardware_lpm(udev, 1);
	usb_unlocked_enable_lpm(udev);
	usb_enable_ltm(udev);
	usb_release_bos_descriptor(udev);
	udev->bos = bos;
	return 0;

re_enumerate:
	usb_release_bos_descriptor(udev);
	udev->bos = bos;
re_enumerate_no_bos:
	/* LPM state doesn't matter when we're about to destroy the device. */
	hub_port_logical_disconnect(parent_hub, port1);
	return -ENODEV;
}
