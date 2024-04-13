Win32ErrorHandler(const char* module, const char* fmt, va_list ap)
{
#ifndef TIF_PLATFORM_CONSOLE
	LPTSTR szTitle;
	LPTSTR szTmp;
	LPCTSTR szTitleText = "%s Error";
	LPCTSTR szDefaultModule = "LIBTIFF";
	LPCTSTR szTmpModule = (module == NULL) ? szDefaultModule : module;
        SIZE_T nBufSize = (strlen(szTmpModule) +
                        strlen(szTitleText) + strlen(fmt) + 256)*sizeof(char);

	if ((szTitle = (LPTSTR)LocalAlloc(LMEM_FIXED, nBufSize)) == NULL)
		return;
	sprintf(szTitle, szTitleText, szTmpModule);
	szTmp = szTitle + (strlen(szTitle)+2)*sizeof(char);
	vsnprintf(szTmp, nBufSize-(strlen(szTitle)+2)*sizeof(char), fmt, ap);
	MessageBoxA(GetFocus(), szTmp, szTitle, MB_OK | MB_ICONEXCLAMATION);
	LocalFree(szTitle);
	return;
#else
	if (module != NULL)
		fprintf(stderr, "%s: ", module);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, ".\n");
#endif        
}
