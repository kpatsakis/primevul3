static void usb_debugfs_init(void)
{
	usb_debug_root = debugfs_create_dir("usb", NULL);
	debugfs_create_file("devices", 0444, usb_debug_root, NULL,
			    &usbfs_devices_fops);
}
