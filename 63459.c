	bool testUnescapingHelper(const wchar_t * input, const wchar_t * output,
			bool plusToSpace = false, UriBreakConversion breakConversion = URI_BR_DONT_TOUCH) {
		wchar_t * working = new wchar_t[URI_STRLEN(input) + 1];
		wcscpy(working, input);
		const wchar_t * newTermZero = uriUnescapeInPlaceExW(working,
				plusToSpace ? URI_TRUE : URI_FALSE, breakConversion);
		const bool success = ((newTermZero == working + wcslen(output))
				&& !wcscmp(working, output));
		delete[] working;
		return success;
	}
