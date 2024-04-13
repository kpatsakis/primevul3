sf_version_string (void)
{
#if	ENABLE_EXPERIMENTAL_CODE
	return PACKAGE_NAME "-" PACKAGE_VERSION "-exp" ;
#else
	return PACKAGE_NAME "-" PACKAGE_VERSION ;
#endif
}
