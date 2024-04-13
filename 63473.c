	void testUriUserInfoHostPort23_Bug3510198_3() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;

		int res;
		res = uriParseUriA(&stateA, "http" "://" "user:!$&'()*+,;=" "@" "host" "/");
		TEST_ASSERT(URI_SUCCESS == res);
		TEST_ASSERT(!memcmp(uriA.userInfo.first, "user:!$&'()*+,;=", 16 * sizeof(char)));
		TEST_ASSERT(uriA.userInfo.afterLast - uriA.userInfo.first == 16);
		TEST_ASSERT(!memcmp(uriA.hostText.first, "host", 4 * sizeof(char)));
		TEST_ASSERT(uriA.hostText.afterLast - uriA.hostText.first == 4);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);
		uriFreeUriMembersA(&uriA);

	}
