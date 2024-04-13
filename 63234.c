static void announce_device(struct usb_device *udev)
{
	u16 bcdDevice = le16_to_cpu(udev->descriptor.bcdDevice);

	dev_info(&udev->dev,
		"New USB device found, idVendor=%04x, idProduct=%04x, bcdDevice=%2x.%02x\n",
		le16_to_cpu(udev->descriptor.idVendor),
		le16_to_cpu(udev->descriptor.idProduct),
		bcdDevice >> 8, bcdDevice & 0xff);
	dev_info(&udev->dev,
		"New USB device strings: Mfr=%d, Product=%d, SerialNumber=%d\n",
		udev->descriptor.iManufacturer,
		udev->descriptor.iProduct,
		udev->descriptor.iSerialNumber);
	show_string(udev, "Product", udev->product);
	show_string(udev, "Manufacturer", udev->manufacturer);
	show_string(udev, "SerialNumber", udev->serial);
}
