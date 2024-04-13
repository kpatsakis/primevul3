	void testUri() {
		UriParserStateA stateA;
		UriParserStateW stateW;
		UriUriA uriA;
		UriUriW uriW;

		stateA.uri = &uriA;
		stateW.uri = &uriW;

		TEST_ASSERT(0 == uriParseUriA(&stateA, "//user:pass@[::1]:80/segment/index.html?query#frag"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://[::1]:80/segment/index.html?query#frag"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://user:pass@[::1]/segment/index.html?query#frag"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://user:pass@[::1]:80?query#frag"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://user:pass@[::1]:80/segment/index.html#frag"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://user:pass@[::1]:80/segment/index.html?query"));
		uriFreeUriMembersA(&uriA);

		TEST_ASSERT(0 == uriParseUriA(&stateA, "ftp://host:21/gnu/"));
		uriFreeUriMembersA(&uriA);

		TEST_ASSERT(0 == uriParseUriA(&stateA, "one/two/three"));
		TEST_ASSERT(!uriA.absolutePath);
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "/one/two/three"));
		TEST_ASSERT(uriA.absolutePath);
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "//user:pass@localhost/one/two/three"));
		uriFreeUriMembersA(&uriA);

		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://www.example.com/"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriW(&stateW, L"http://www.example.com/"));
		uriFreeUriMembersW(&uriW);

		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://sourceforge.net/projects/uriparser/"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://sourceforge.net/project/platformdownload.php?group_id=182840"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "mailto:test@example.com"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "../../"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "/"));
		TEST_ASSERT(uriA.absolutePath)
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, ""));
		TEST_ASSERT(!uriA.absolutePath)
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 == uriParseUriA(&stateA, "file:///bin/bash"));
		uriFreeUriMembersA(&uriA);

		TEST_ASSERT(0 == uriParseUriA(&stateA, "http://www.example.com/name%20with%20spaces/"));
		uriFreeUriMembersA(&uriA);
		TEST_ASSERT(0 != uriParseUriA(&stateA, "http://www.example.com/name with spaces/"));
		uriFreeUriMembersA(&uriA);
	}
