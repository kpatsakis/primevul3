	void testUriComponents() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		const char * const input = "http" "://" "sourceforge.net" "/" "project" "/"
				"platformdownload.php" "?" "group_id=182840";
		TEST_ASSERT(0 == uriParseUriA(&stateA, input));

		TEST_ASSERT(uriA.scheme.first == input);
		TEST_ASSERT(uriA.scheme.afterLast == input + 4);
		TEST_ASSERT(uriA.userInfo.first == NULL);
		TEST_ASSERT(uriA.userInfo.afterLast == NULL);
		TEST_ASSERT(uriA.hostText.first == input + 4 + 3);
		TEST_ASSERT(uriA.hostText.afterLast == input + 4 + 3 + 15);
		TEST_ASSERT(uriA.hostData.ipFuture.first == NULL);
		TEST_ASSERT(uriA.hostData.ipFuture.afterLast == NULL);
		TEST_ASSERT(uriA.portText.first == NULL);
		TEST_ASSERT(uriA.portText.afterLast == NULL);

		TEST_ASSERT(uriA.pathHead->text.first == input + 4 + 3 + 15 + 1);
		TEST_ASSERT(uriA.pathHead->text.afterLast == input + 4 + 3 + 15 + 1 + 7);
		TEST_ASSERT(uriA.pathHead->next->text.first == input + 4 + 3 + 15 + 1 + 7 + 1);
		TEST_ASSERT(uriA.pathHead->next->text.afterLast == input + 4 + 3 + 15 + 1 + 7 + 1 + 20);
		TEST_ASSERT(uriA.pathHead->next->next == NULL);
		TEST_ASSERT(uriA.pathTail == uriA.pathHead->next);

		TEST_ASSERT(uriA.query.first == input + 4 + 3 + 15 + 1 + 7 + 1 + 20 + 1);
		TEST_ASSERT(uriA.query.afterLast == input + 4 + 3 + 15 + 1 + 7 + 1 + 20 + 1 + 15);
		TEST_ASSERT(uriA.fragment.first == NULL);
		TEST_ASSERT(uriA.fragment.afterLast == NULL);
		uriFreeUriMembersA(&uriA);
	}
