	void testEqualsHelper(const char * uri_to_test) {
		UriParserStateA state;
		UriUriA uriOne;
		UriUriA uriTwo;
		state.uri = &uriOne;
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&state, uri_to_test));
		state.uri = &uriTwo;
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&state, uri_to_test));
		TEST_ASSERT(URI_TRUE == uriEqualsUriA(&uriOne, &uriTwo));
		uriFreeUriMembersA(&uriOne);
		uriFreeUriMembersA(&uriTwo);
	}
