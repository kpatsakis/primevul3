 	void testFreeCrash_Bug20080827() {
 		char const * const sourceUri = "abc";
 		char const * const baseUri = "http://www.example.org/";

		int res;
		UriParserStateA state;
		UriUriA absoluteDest;
		UriUriA relativeSource;
		UriUriA absoluteBase;

		state.uri = &relativeSource;
		res = uriParseUriA(&state, sourceUri);
		TEST_ASSERT(res == URI_SUCCESS);

		state.uri = &absoluteBase;
		res = uriParseUriA(&state, baseUri);
		TEST_ASSERT(res == URI_SUCCESS);

		res = uriRemoveBaseUriA(&absoluteDest, &relativeSource, &absoluteBase, URI_FALSE);
		TEST_ASSERT(res == URI_ERROR_REMOVEBASE_REL_SOURCE);

		uriFreeUriMembersA(&relativeSource);
		uriFreeUriMembersA(&absoluteBase);
		uriFreeUriMembersA(&absoluteDest); // Crashed here
	}
