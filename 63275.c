static int usb_bus_notify(struct notifier_block *nb, unsigned long action,
		void *data)
{
	struct device *dev = data;

	switch (action) {
	case BUS_NOTIFY_ADD_DEVICE:
		if (dev->type == &usb_device_type)
			(void) usb_create_sysfs_dev_files(to_usb_device(dev));
		else if (dev->type == &usb_if_device_type)
			usb_create_sysfs_intf_files(to_usb_interface(dev));
		break;

	case BUS_NOTIFY_DEL_DEVICE:
		if (dev->type == &usb_device_type)
			usb_remove_sysfs_dev_files(to_usb_device(dev));
		else if (dev->type == &usb_if_device_type)
			usb_remove_sysfs_intf_files(to_usb_interface(dev));
		break;
	}
	return 0;
}
