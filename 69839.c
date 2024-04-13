NTSTATUS WriteRegistryConfigFlags (uint32 flags)
{
	UNICODE_STRING name;
	RtlInitUnicodeString (&name, L"\\REGISTRY\\MACHINE\\SYSTEM\\CurrentControlSet\\Services\\veracrypt");

	return TCWriteRegistryKey (&name, TC_DRIVER_CONFIG_REG_VALUE_NAME, REG_DWORD, &flags, sizeof (flags));
}
