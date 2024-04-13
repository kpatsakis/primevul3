	void testUriUserInfoHostPort23_Bug3510198_2() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;

		int res;
		res = uriParseUriA(&stateA, "http" "://" "%2Fuser:%2F21" "@" "host" "/");
		TEST_ASSERT(URI_SUCCESS == res);
		TEST_ASSERT(!memcmp(uriA.userInfo.first, "%2Fuser:%2F21", 13 * sizeof(char)));
		TEST_ASSERT(uriA.userInfo.afterLast - uriA.userInfo.first == 13);
		TEST_ASSERT(!memcmp(uriA.hostText.first, "host", 4 * sizeof(char)));
		TEST_ASSERT(uriA.hostText.afterLast - uriA.hostText.first == 4);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}
