	bool testEscapingHelper(const wchar_t * in, const wchar_t * expectedOut,
			bool spaceToPlus = false, bool normalizeBreaks = false) {
		wchar_t * const buffer = new wchar_t[(normalizeBreaks ? 6 : 3)
				* wcslen(in) + 1];
		if (uriEscapeW(in, buffer, spaceToPlus, normalizeBreaks)
			!= buffer + wcslen(expectedOut)) {
			delete [] buffer;
			return false;
		}

		const bool equal = !wcscmp(buffer, expectedOut);
		delete [] buffer;
		return equal;
	}
