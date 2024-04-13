	void testUriUserInfoHostPort23_Bug3510198_4() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;

		int res;
		res = uriParseUriA(&stateA, "http" "://" "!$&'()*+,;=:password" "@" "host" "/");
		TEST_ASSERT(URI_SUCCESS == res);
		TEST_ASSERT(!memcmp(uriA.userInfo.first, "!$&'()*+,;=:password", 20 * sizeof(char)));
		TEST_ASSERT(uriA.userInfo.afterLast - uriA.userInfo.first == 20);
		TEST_ASSERT(!memcmp(uriA.hostText.first, "host", 4 * sizeof(char)));
		TEST_ASSERT(uriA.hostText.afterLast - uriA.hostText.first == 4);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}
