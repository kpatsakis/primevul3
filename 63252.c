static void hub_usb3_port_prepare_disable(struct usb_hub *hub,
					  struct usb_port *port_dev)
{
	struct usb_device *udev = port_dev->child;
	int ret;

	if (udev && udev->port_is_suspended && udev->do_remote_wakeup) {
		ret = hub_set_port_link_state(hub, port_dev->portnum,
					      USB_SS_PORT_LS_U0);
		if (!ret) {
			msleep(USB_RESUME_TIMEOUT);
			ret = usb_disable_remote_wakeup(udev);
		}
		if (ret)
			dev_warn(&udev->dev,
				 "Port disable: can't disable remote wake\n");
		udev->do_remote_wakeup = 0;
	}
}
