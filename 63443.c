	bool testNormalizeSyntaxHelper(const wchar_t * uriText, const wchar_t * expectedNormalized,
			unsigned int mask = static_cast<unsigned int>(-1)) {
		UriParserStateW stateW;
		int res;

		UriUriW testUri;
		stateW.uri = &testUri;
		res = uriParseUriW(&stateW, uriText);
		if (res != 0) {
			uriFreeUriMembersW(&testUri);
			return false;
		}

		UriUriW expectedUri;
		stateW.uri = &expectedUri;
		res = uriParseUriW(&stateW, expectedNormalized);
		if (res != 0) {
			uriFreeUriMembersW(&testUri);
			uriFreeUriMembersW(&expectedUri);
			return false;
		}

		res = uriNormalizeSyntaxExW(&testUri, mask);
		if (res != 0) {
			uriFreeUriMembersW(&testUri);
			uriFreeUriMembersW(&expectedUri);
			return false;
		}

		bool equalAfter = (URI_TRUE == uriEqualsUriW(&testUri, &expectedUri));

		res = uriNormalizeSyntaxExW(&testUri, mask);
		if (res != 0) {
			uriFreeUriMembersW(&testUri);
			uriFreeUriMembersW(&expectedUri);
			return false;
		}

		equalAfter = equalAfter
				&& (URI_TRUE == uriEqualsUriW(&testUri, &expectedUri));

		uriFreeUriMembersW(&testUri);
		uriFreeUriMembersW(&expectedUri);
		return equalAfter;
	}
