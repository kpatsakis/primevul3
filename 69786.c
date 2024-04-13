static PDEVICE_OBJECT FindVolumeWithHighestUniqueId (int maxUniqueId)
{
	PDEVICE_OBJECT highestIdDevice = NULL;
	int highestId = -1;
	int drive;

	for (drive = MIN_MOUNTED_VOLUME_DRIVE_NUMBER; drive <= MAX_MOUNTED_VOLUME_DRIVE_NUMBER; ++drive)
	{
		PDEVICE_OBJECT device = GetVirtualVolumeDeviceObject (drive);
		if (device)
		{
			PEXTENSION extension = (PEXTENSION) device->DeviceExtension;
			if (extension->UniqueVolumeId > highestId && extension->UniqueVolumeId <= maxUniqueId)
			{
				highestId = extension->UniqueVolumeId;
				highestIdDevice = device;
			}
		}
	}

	return highestIdDevice;
}
