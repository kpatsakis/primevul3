void TCGetNTNameFromNumber (LPWSTR ntname, int cbNtName, int nDriveNo)
{
	WCHAR tmp[2] =
	{0, 0};
	int j = nDriveNo + (WCHAR) 'A';

	tmp[0] = (short) j;
	RtlStringCbCopyW (ntname, cbNtName,(LPWSTR) NT_MOUNT_PREFIX);
	RtlStringCbCatW (ntname, cbNtName, tmp);
}
