BOOL UserCanAccessDriveDevice ()
{
	UNICODE_STRING name;
	RtlInitUnicodeString (&name, L"\\Device\\MountPointManager");

	return IsAccessibleByUser (&name, FALSE);
}
