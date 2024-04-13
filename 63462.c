	void testUriComponents_Bug20070701() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		const char * const input = "a" ":" "b";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.scheme.first == input);
		TEST_ASSERT(uriA.scheme.afterLast == input + 1);
		TEST_ASSERT(uriA.userInfo.first == NULL);
		TEST_ASSERT(uriA.userInfo.afterLast == NULL);
		TEST_ASSERT(uriA.hostText.first == NULL);
		TEST_ASSERT(uriA.hostText.afterLast == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.first == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.afterLast == NULL);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);

		TEST_ASSERT(uriA.pathHead->text.first == input + 1 + 1);
		TEST_ASSERT(uriA.pathHead->text.afterLast == input + 1 + 1 + 1);
		TEST_ASSERT(uriA.pathHead->next == NULL);
		TEST_ASSERT(uriA.pathTail == uriA.pathHead);

		TEST_ASSERT(uriA.query.first == NULL);
		TEST_ASSERT(uriA.query.afterLast == NULL);
		TEST_ASSERT(uriA.fragment.first == NULL);
		TEST_ASSERT(uriA.fragment.afterLast == NULL);

		TEST_ASSERT(!uriA.absolutePath);
		uriFreeUriMembersA(&uriA);
	}
