static int usb_dev_poweroff(struct device *dev)
{
	return usb_suspend(dev, PMSG_HIBERNATE);
}
