	void testTrailingSlash() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		const char * const input = "abc" "/";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.pathHead->text.first == input);
		TEST_ASSERT(uriA.pathHead->text.afterLast == input + 3);
		TEST_ASSERT(uriA.pathHead->next->text.first == uriA.pathHead->next->text.afterLast);
		TEST_ASSERT(uriA.pathHead->next->next == NULL);
		TEST_ASSERT(uriA.pathTail == uriA.pathHead->next);
		uriFreeUriMembersA(&uriA);
	}
