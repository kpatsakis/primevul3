static void usb_debugfs_cleanup(void)
{
	debugfs_remove_recursive(usb_debug_root);
}
