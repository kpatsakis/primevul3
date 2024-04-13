	void testUriUserInfoHostPort22_Bug1948038() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;

		int res;

		res = uriParseUriA(&stateA, "http://user:21@host/");
		TEST_ASSERT(URI_SUCCESS == res);
		TEST_ASSERT(!memcmp(uriA.userInfo.first, "user:21", 7 * sizeof(char)));
		TEST_ASSERT(uriA.userInfo.afterLast - uriA.userInfo.first == 7);
		TEST_ASSERT(!memcmp(uriA.hostText.first, "host", 4 * sizeof(char)));
		TEST_ASSERT(uriA.hostText.afterLast - uriA.hostText.first == 4);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);
		uriFreeUriMembersA(&uriA);

		res = uriParseUriA(&stateA, "http://user:1234@192.168.0.1:1234/foo.com");
		TEST_ASSERT(URI_SUCCESS == res);
		uriFreeUriMembersA(&uriA);

		res = uriParseUriA(&stateA, "http://moo:21@moo:21@moo/");
		TEST_ASSERT(URI_ERROR_SYNTAX == res);
		uriFreeUriMembersA(&uriA);

		res = uriParseUriA(&stateA, "http://moo:21@moo:21@moo:21/");
		TEST_ASSERT(URI_ERROR_SYNTAX == res);
		uriFreeUriMembersA(&uriA);
	}
