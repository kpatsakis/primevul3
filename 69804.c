void OnShutdownPending ()
{
	UNMOUNT_STRUCT unmount;
	memset (&unmount, 0, sizeof (unmount));
	unmount.ignoreOpenFiles = TRUE;

	while (SendDeviceIoControlRequest (RootDeviceObject, TC_IOCTL_DISMOUNT_ALL_VOLUMES, &unmount, sizeof (unmount), &unmount, sizeof (unmount)) == STATUS_INSUFFICIENT_RESOURCES || unmount.HiddenVolumeProtectionTriggered)
		unmount.HiddenVolumeProtectionTriggered = FALSE;

	while (SendDeviceIoControlRequest (RootDeviceObject, TC_IOCTL_WIPE_PASSWORD_CACHE, NULL, 0, NULL, 0) == STATUS_INSUFFICIENT_RESOURCES);
}
