	void testCrash_MakeOwner_Bug20080207() {
		UriParserStateA state;
		UriUriA sourceUri;
		state.uri = &sourceUri;
		const char * const sourceUriString = "http://user:pass@somehost.com:80/";
		if (uriParseUriA(&state, sourceUriString) != 0) {
			TEST_ASSERT(false);
		}
		if (uriNormalizeSyntaxA(&sourceUri) != 0) {
			TEST_ASSERT(false);
		}
		uriFreeUriMembersA(&sourceUri);
		TEST_ASSERT(true);
	}
