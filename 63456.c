	bool testToStringHelper(const wchar_t * text) {
		UriParserStateW state;
		UriUriW uri;
		state.uri = &uri;
		int res = uriParseUriW(&state, text);
		if (res != 0) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		wchar_t shouldbeTheSame[1024 * 8];
		res = uriToStringW(shouldbeTheSame, &uri, 1024 * 8, NULL);
		if (res != 0) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		bool equals = (0 == wcscmp(shouldbeTheSame, text));
		if (!equals) {
#ifdef HAVE_WPRINTF
			wprintf(L"\n\n\nExpected: \"%s\"\nReceived: \"%s\"\n\n\n", text, shouldbeTheSame);
#endif
		}

		const int len = static_cast<int>(wcslen(text));
		int charsWritten;
		res = uriToStringW(shouldbeTheSame, &uri, len + 1, &charsWritten);
		if ((res != 0) || (charsWritten != len + 1)) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		res = uriToStringW(shouldbeTheSame, &uri, len, &charsWritten);
		if ((res == 0) || (charsWritten >= len + 1)) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		uriFreeUriMembersW(&uri);
		return equals;
	}
