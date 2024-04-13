static int usb_dev_thaw(struct device *dev)
{
	return usb_resume(dev, PMSG_THAW);
}
