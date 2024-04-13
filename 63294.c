int usb_get_current_frame_number(struct usb_device *dev)
{
	return usb_hcd_get_frame_number(dev);
}
