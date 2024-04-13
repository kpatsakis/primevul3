NTSTATUS DriverEntry (PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	PKEY_VALUE_PARTIAL_INFORMATION startKeyValue;
	LONG version;
	int i;

	Dump ("DriverEntry " TC_APP_NAME " " VERSION_STRING "\n");

	DetectX86Features ();

	PsGetVersion (&OsMajorVersion, &OsMinorVersion, NULL, NULL);

	Dump ("OsMajorVersion=%d OsMinorVersion=%d\n", OsMajorVersion, OsMinorVersion);

	if ((OsMajorVersion > 6) || (OsMajorVersion == 6 && OsMinorVersion >= 2))
	{
		ExDefaultNonPagedPoolType = (POOL_TYPE) NonPagedPoolNx;
		ExDefaultMdlProtection = MdlMappingNoExecute;
	}

	if ((OsMajorVersion > 6) || (OsMajorVersion == 6 && OsMinorVersion >= 1))
	{
		UNICODE_STRING saveFuncName, restoreFuncName;
		RtlInitUnicodeString(&saveFuncName, L"KeSaveExtendedProcessorState");
		RtlInitUnicodeString(&restoreFuncName, L"KeRestoreExtendedProcessorState");
		KeSaveExtendedProcessorStatePtr = (KeSaveExtendedProcessorStateFn) MmGetSystemRoutineAddress(&saveFuncName);
		KeRestoreExtendedProcessorStatePtr = (KeRestoreExtendedProcessorStateFn) MmGetSystemRoutineAddress(&restoreFuncName);
	}

	if (NT_SUCCESS (TCDeviceIoControl (NT_ROOT_PREFIX, TC_IOCTL_GET_DRIVER_VERSION, NULL, 0, &version, sizeof (version))))
		return DumpFilterEntry ((PFILTER_EXTENSION) DriverObject, (PFILTER_INITIALIZATION_DATA) RegistryPath);

	TCDriverObject = DriverObject;
	memset (VirtualVolumeDeviceObjects, 0, sizeof (VirtualVolumeDeviceObjects));

	ReadRegistryConfigFlags (TRUE);
	EncryptionThreadPoolStart (EncryptionThreadPoolFreeCpuCountLimit);
	SelfTestsPassed = AutoTestAlgorithms();


	if (NT_SUCCESS (TCReadRegistryKey (RegistryPath, L"Start", &startKeyValue)))
	{
		if (startKeyValue->Type == REG_DWORD && *((uint32 *) startKeyValue->Data) == SERVICE_BOOT_START)
		{
			if (!SelfTestsPassed)
			{
				DisableCPUExtendedFeatures ();
				SelfTestsPassed = AutoTestAlgorithms();

				if (!SelfTestsPassed)
					TC_BUG_CHECK (STATUS_INVALID_PARAMETER);
			}

			LoadBootArguments();
			VolumeClassFilterRegistered = IsVolumeClassFilterRegistered();

			DriverObject->DriverExtension->AddDevice = DriverAddDevice;
		}

		TCfree (startKeyValue);
	}

	for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; ++i)
	{
		DriverObject->MajorFunction[i] = TCDispatchQueueIRP;
	}

	DriverObject->DriverUnload = TCUnloadDriver;
	return TCCreateRootDeviceObject (DriverObject);
}
