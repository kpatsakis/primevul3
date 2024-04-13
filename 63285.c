static char *usb_devnode(struct device *dev,
			 umode_t *mode, kuid_t *uid, kgid_t *gid)
{
	struct usb_device *usb_dev;

	usb_dev = to_usb_device(dev);
	return kasprintf(GFP_KERNEL, "bus/usb/%03d/%03d",
			 usb_dev->bus->busnum, usb_dev->devnum);
}
