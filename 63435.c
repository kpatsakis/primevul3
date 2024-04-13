	void testFilenameUriConversionHelper(const wchar_t * filename,
			const wchar_t * uriString, bool forUnix,
			const wchar_t * expectedUriString = NULL) {
		const int prefixLen = forUnix ? 7 : 8;
		if (! expectedUriString) {
			expectedUriString = uriString;
		}

		const size_t uriBufferLen = prefixLen + 3 * wcslen(filename) + 1;
		wchar_t * uriBuffer = new wchar_t[uriBufferLen];
		if (forUnix) {
			uriUnixFilenameToUriStringW(filename, uriBuffer);
		} else {
			uriWindowsFilenameToUriStringW(filename, uriBuffer);
		}
#ifdef HAVE_WPRINTF
#endif
		TEST_ASSERT(!wcscmp(uriBuffer, expectedUriString));
		delete [] uriBuffer;

		const size_t filenameBufferLen = wcslen(uriString) + 1;
		wchar_t * filenameBuffer = new wchar_t[filenameBufferLen];
		if (forUnix) {
			uriUriStringToUnixFilenameW(uriString, filenameBuffer);
		} else {
			uriUriStringToWindowsFilenameW(uriString, filenameBuffer);
		}
#ifdef HAVE_WPRINTF
#endif
		TEST_ASSERT(!wcscmp(filenameBuffer, filename));
		delete [] filenameBuffer;
	}
