	void testCrash_FreeUriMembers_Bug20080116() {
		UriParserStateA state;
		UriUriA uri;
		state.uri = &uri;
		uriParseUriA(&state, "http://test/?");
		uriNormalizeSyntaxA(&uri);
		uriFreeUriMembersA(&uri);

		TEST_ASSERT(true);
	}
