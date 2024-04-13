PDEVICE_OBJECT GetVirtualVolumeDeviceObject (int driveNumber)
{
	if (driveNumber < MIN_MOUNTED_VOLUME_DRIVE_NUMBER || driveNumber > MAX_MOUNTED_VOLUME_DRIVE_NUMBER)
		return NULL;

	return VirtualVolumeDeviceObjects[driveNumber];
}
