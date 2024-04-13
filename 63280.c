static int usb_dev_restore(struct device *dev)
{
	return usb_resume(dev, PMSG_RESTORE);
}
