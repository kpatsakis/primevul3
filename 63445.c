	void testParseInvalid_Bug16() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		const char * const input = "A>B";

		const int res = uriParseUriA(&stateA, input);

		TEST_ASSERT(res == URI_ERROR_SYNTAX);
		TEST_ASSERT(stateA.errorPos == input + 1);
		TEST_ASSERT(stateA.errorCode == URI_ERROR_SYNTAX);  /* failed previously */

		uriFreeUriMembersA(&uriA);
	}
