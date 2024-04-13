static void usb_release_dev(struct device *dev)
{
	struct usb_device *udev;
	struct usb_hcd *hcd;

	udev = to_usb_device(dev);
	hcd = bus_to_hcd(udev->bus);

	usb_destroy_configuration(udev);
	usb_release_bos_descriptor(udev);
	of_node_put(dev->of_node);
	usb_put_hcd(hcd);
	kfree(udev->product);
	kfree(udev->manufacturer);
	kfree(udev->serial);
	kfree(udev);
}
