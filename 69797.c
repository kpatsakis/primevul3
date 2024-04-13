BOOL IsOSAtLeast (OSVersionEnum reqMinOS)
{
	/* When updating this function, update IsOSVersionAtLeast() in Dlgcode.c too. */

	ULONG major = 0, minor = 0;

	ASSERT (OsMajorVersion != 0);

	switch (reqMinOS)
	{
	case WIN_2000:			major = 5; minor = 0; break;
	case WIN_XP:			major = 5; minor = 1; break;
	case WIN_SERVER_2003:	major = 5; minor = 2; break;
	case WIN_VISTA:			major = 6; minor = 0; break;
	case WIN_7:				major = 6; minor = 1; break;
	case WIN_8:				major = 6; minor = 2; break;
	case WIN_8_1:			major = 6; minor = 3; break;
	case WIN_10:			major = 10; minor = 0; break;

	default:
		TC_THROW_FATAL_EXCEPTION;
		break;
	}

	return ((OsMajorVersion << 16 | OsMinorVersion << 8)
		>= (major << 16 | minor << 8));
}
