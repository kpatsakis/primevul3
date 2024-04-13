	void testUriUserInfoHostPort23_Bug3510198_related_12() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;

		int res;
		res = uriParseUriA(&stateA, "http" "://" "%2Fhost" "/");
		TEST_ASSERT(URI_SUCCESS == res);
		TEST_ASSERT(uriA.userInfo.afterLast == NULL);
		TEST_ASSERT(uriA.userInfo.first == NULL);
		TEST_ASSERT(!memcmp(uriA.hostText.first, "%2Fhost", 7 * sizeof(char)));
		TEST_ASSERT(uriA.hostText.afterLast - uriA.hostText.first == 7);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}
