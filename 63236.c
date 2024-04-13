static int get_hub_descriptor(struct usb_device *hdev,
		struct usb_hub_descriptor *desc)
{
	int i, ret, size;
	unsigned dtype;

	if (hub_is_superspeed(hdev)) {
		dtype = USB_DT_SS_HUB;
		size = USB_DT_SS_HUB_SIZE;
	} else {
		dtype = USB_DT_HUB;
		size = sizeof(struct usb_hub_descriptor);
	}

	for (i = 0; i < 3; i++) {
		ret = usb_control_msg(hdev, usb_rcvctrlpipe(hdev, 0),
			USB_REQ_GET_DESCRIPTOR, USB_DIR_IN | USB_RT_HUB,
			dtype << 8, 0, desc, size,
			USB_CTRL_GET_TIMEOUT);
		if (hub_is_superspeed(hdev)) {
			if (ret == size)
				return ret;
		} else if (ret >= USB_DT_HUB_NONVAR_SIZE + 2) {
			/* Make sure we have the DeviceRemovable field. */
			size = USB_DT_HUB_NONVAR_SIZE + desc->bNbrPorts / 8 + 1;
			if (ret < size)
				return -EMSGSIZE;
			return ret;
		}
	}
	return -EINVAL;
}
