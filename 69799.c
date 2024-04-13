BOOL IsVolumeClassFilterRegistered ()
{
	UNICODE_STRING name;
	NTSTATUS status;
	BOOL registered = FALSE;

	PKEY_VALUE_PARTIAL_INFORMATION data;

	RtlInitUnicodeString (&name, L"\\REGISTRY\\MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{71A27CDD-812A-11D0-BEC7-08002BE2092F}");
	status = TCReadRegistryKey (&name, L"UpperFilters", &data);

	if (NT_SUCCESS (status))
	{
		if (data->Type == REG_MULTI_SZ && data->DataLength >= 9 * sizeof (wchar_t))
		{
			ULONG i;
			for (i = 0; i <= data->DataLength - 9 * sizeof (wchar_t); ++i)
			{
				if (memcmp (data->Data + i, L"veracrypt", 9 * sizeof (wchar_t)) == 0)
				{
					Dump ("Volume class filter active\n");
					registered = TRUE;
					break;
				}
			}
		}

		TCfree (data);
	}

	return registered;
}
