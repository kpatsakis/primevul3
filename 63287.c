static void __exit usb_exit(void)
{
	/* This will matter if shutdown/reboot does exitcalls. */
	if (usb_disabled())
		return;

	usb_release_quirk_list();
	usb_deregister_device_driver(&usb_generic_driver);
	usb_major_cleanup();
	usb_deregister(&usbfs_driver);
	usb_devio_cleanup();
	usb_hub_cleanup();
	bus_unregister_notifier(&usb_bus_type, &usb_bus_nb);
	bus_unregister(&usb_bus_type);
	usb_acpi_unregister();
	usb_debugfs_cleanup();
	idr_destroy(&usb_bus_idr);
}
