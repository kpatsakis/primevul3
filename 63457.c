	void testToString_Bug1950126() {
		UriParserStateW state;
		UriUriW uriOne;
		UriUriW uriTwo;
		const wchar_t * const uriOneString = L"http://e.com/";
		const wchar_t * const uriTwoString = L"http://e.com";
		state.uri = &uriOne;
		TEST_ASSERT(URI_SUCCESS == uriParseUriW(&state, uriOneString));
		state.uri = &uriTwo;
		TEST_ASSERT(URI_SUCCESS == uriParseUriW(&state, uriTwoString));
		TEST_ASSERT(URI_FALSE == uriEqualsUriW(&uriOne, &uriTwo));
		uriFreeUriMembersW(&uriOne);
		uriFreeUriMembersW(&uriTwo);

		TEST_ASSERT(testToStringHelper(uriOneString));
		TEST_ASSERT(testToStringHelper(uriTwoString));
	}
