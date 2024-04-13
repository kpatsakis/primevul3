	void testUriHostRegname() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		const char * const input = "http" "://" "example.com";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.hostText.first == input + 4 + 3);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 11);
		TEST_ASSERT(uriA.hostData.ip4 == NULL);
		TEST_ASSERT(uriA.hostData.ip6 == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.first == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}
