NTSTATUS ReadRegistryConfigFlags (BOOL driverEntry)
{
	PKEY_VALUE_PARTIAL_INFORMATION data;
	UNICODE_STRING name;
	NTSTATUS status;
	uint32 flags = 0;

	RtlInitUnicodeString (&name, L"\\REGISTRY\\MACHINE\\SYSTEM\\CurrentControlSet\\Services\\veracrypt");
	status = TCReadRegistryKey (&name, TC_DRIVER_CONFIG_REG_VALUE_NAME, &data);

	if (NT_SUCCESS (status))
	{
		if (data->Type == REG_DWORD)
		{
			flags = *(uint32 *) data->Data;
			Dump ("Configuration flags = 0x%x\n", flags);

			if (driverEntry)
			{
				if (flags & (TC_DRIVER_CONFIG_CACHE_BOOT_PASSWORD | TC_DRIVER_CONFIG_CACHE_BOOT_PASSWORD_FOR_SYS_FAVORITES))
					CacheBootPassword = TRUE;

				if (flags & TC_DRIVER_CONFIG_DISABLE_NONADMIN_SYS_FAVORITES_ACCESS)
					NonAdminSystemFavoritesAccessDisabled = TRUE;

				if (flags & TC_DRIVER_CONFIG_CACHE_BOOT_PIM)
					CacheBootPim = TRUE;

				if (flags & VC_DRIVER_CONFIG_BLOCK_SYS_TRIM)
					BlockSystemTrimCommand = TRUE;
			}

			EnableHwEncryption ((flags & TC_DRIVER_CONFIG_DISABLE_HARDWARE_ENCRYPTION) ? FALSE : TRUE);

			EnableExtendedIoctlSupport = (flags & TC_DRIVER_CONFIG_ENABLE_EXTENDED_IOCTL)? TRUE : FALSE;
			AllowTrimCommand = (flags & VC_DRIVER_CONFIG_ALLOW_NONSYS_TRIM)? TRUE : FALSE;
			AllowWindowsDefrag = (flags & VC_DRIVER_CONFIG_ALLOW_WINDOWS_DEFRAG)? TRUE : FALSE;
		}
		else
			status = STATUS_INVALID_PARAMETER;

		TCfree (data);
	}

	if (driverEntry && NT_SUCCESS (TCReadRegistryKey (&name, TC_ENCRYPTION_FREE_CPU_COUNT_REG_VALUE_NAME, &data)))
	{
		if (data->Type == REG_DWORD)
			EncryptionThreadPoolFreeCpuCountLimit = *(uint32 *) data->Data;

		TCfree (data);
	}

	return status;
}
