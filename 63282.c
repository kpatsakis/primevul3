static int usb_dev_suspend(struct device *dev)
{
	return usb_suspend(dev, PMSG_SUSPEND);
}
