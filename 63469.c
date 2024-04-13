	void testUriUserInfoHostPort2() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		const char * const input = "http" "://" "abc:def" "@" "localhost"
				":" "123";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.userInfo.first == input + 4 + 3);
		TEST_ASSERT(uriA.userInfo.afterLast == input + 4 + 3 + 7);
		TEST_ASSERT(uriA.hostText.first == input + 4 + 3 + 7 + 1);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 7 + 1 + 9);
		TEST_ASSERT(uriA.portText.first == input + 4 + 3 + 7 + 1 + 9 + 1);
		TEST_ASSERT(uriA.portText.afterLast == input + 4 + 3 + 7 + 1 + 9 + 1 + 3);
		uriFreeUriMembersA(&uriA);
	}
