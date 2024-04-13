	void testNormalizeCrash_Bug20080224() {
		UriParserStateW stateW;
		int res;
		UriUriW testUri;
		stateW.uri = &testUri;

		res = uriParseUriW(&stateW, L"http://example.org/abc//../def");
		TEST_ASSERT(res == 0);

		res = uriNormalizeSyntaxExW(&testUri, URI_NORMALIZE_SCHEME);
		TEST_ASSERT(res == 0);
		res = uriNormalizeSyntaxExW(&testUri, URI_NORMALIZE_HOST);
		TEST_ASSERT(res == 0);

		res = uriNormalizeSyntaxW(&testUri);
		TEST_ASSERT(res == 0);

		uriFreeUriMembersW(&testUri);
	}
