NTSTATUS DriverAddDevice (PDRIVER_OBJECT driverObject, PDEVICE_OBJECT pdo)
{
#if defined(DEBUG) || defined (DEBUG_TRACE)
	char nameInfoBuffer[128];
	POBJECT_NAME_INFORMATION nameInfo = (POBJECT_NAME_INFORMATION) nameInfoBuffer;
	ULONG nameInfoSize;
	Dump ("AddDevice pdo=%p type=%x name=%ws\n", pdo, pdo->DeviceType, NT_SUCCESS (ObQueryNameString (pdo, nameInfo, sizeof (nameInfoBuffer), &nameInfoSize)) ? nameInfo->Name.Buffer : L"?");
#endif

	if (VolumeClassFilterRegistered && BootArgsValid && BootArgs.HiddenSystemPartitionStart != 0)
	{
		PWSTR interfaceLinks = NULL;
		if (NT_SUCCESS (IoGetDeviceInterfaces (&GUID_DEVINTERFACE_VOLUME, pdo, DEVICE_INTERFACE_INCLUDE_NONACTIVE, &interfaceLinks)) && interfaceLinks)
		{
			if (interfaceLinks[0] != UNICODE_NULL)
			{
				Dump ("Volume pdo=%p interface=%ws\n", pdo, interfaceLinks);
				ExFreePool (interfaceLinks);

				return VolumeFilterAddDevice (driverObject, pdo);
			}

			ExFreePool (interfaceLinks);
		}
	}

	return DriveFilterAddDevice (driverObject, pdo);
}
