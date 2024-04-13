	void testUriUserInfoHostPort6() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		const char * const input = "http" "://" "localhost" ":" "123";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.userInfo.first == NULL);
		TEST_ASSERT(uriA.userInfo.afterLast == NULL);
		TEST_ASSERT(uriA.hostText.first == input + 4 + 3);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 9);
		TEST_ASSERT(uriA.portText.first == input + 4 + 3 + 9 + 1);
		TEST_ASSERT(uriA.portText.afterLast == input + 4 + 3 + 9 + 1 + 3);
		uriFreeUriMembersA(&uriA);
	}
