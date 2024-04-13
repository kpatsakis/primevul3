announcedata_size()
{
	size_t size, i;
	PRINTER *printerinfo;
	DISK_DEVICE *diskinfo;

	size = 8;		/* Header + DeviceCount */

	for (i = 0; i < g_num_devices; i++)
	{
		size += 4;	/* DeviceType */
		size += 4;	/* DeviceId */
		size += 8;	/* PreferredDosName */
		size += 4;	/* DeviceDataLength */

		switch (g_rdpdr_device[i].device_type)
		{
			case DEVICE_TYPE_DISK:
				diskinfo = (DISK_DEVICE *) g_rdpdr_device[i].pdevice_data;
				size += 2 * strlen(diskinfo->name) + 2;
				break;
			case DEVICE_TYPE_PRINTER:
				printerinfo = (PRINTER *) g_rdpdr_device[i].pdevice_data;
				printerinfo->bloblen =
					printercache_load_blob(printerinfo->printer,
							       &(printerinfo->blob));

				size += 0x18;
				size += 2 * strlen(printerinfo->driver) + 2;
				size += 2 * strlen(printerinfo->printer) + 2;
				size += printerinfo->bloblen;
				break;
			default:
				break;
		}
	}

	return size;
}
