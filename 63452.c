	void testRemoveBaseUriHelper(const char * expected,
								const char * absSourceStr,
								const char * absBaseStr) {
		UriParserStateA state;
		UriUriA absSource;
		UriUriA absBase;
		UriUriA dest;

		state.uri = &absSource;
		TEST_ASSERT(uriParseUriA(&state, absSourceStr) == URI_SUCCESS);

		state.uri = &absBase;
		TEST_ASSERT(uriParseUriA(&state, absBaseStr) == URI_SUCCESS);

		TEST_ASSERT(uriRemoveBaseUriA(&dest, &absSource, &absBase, URI_FALSE)
				== URI_SUCCESS);

		int size = 0;
		TEST_ASSERT(uriToStringCharsRequiredA(&dest, &size) == URI_SUCCESS);
		char * const buffer = (char *)malloc(size + 1);
		TEST_ASSERT(buffer);
		TEST_ASSERT(uriToStringA(buffer, &dest, size + 1, &size)
															== URI_SUCCESS);
		if (strcmp(buffer, expected)) {
			printf("Expected \"%s\" but got \"%s\"\n", expected, buffer);
			TEST_ASSERT(0);
		}
		free(buffer);
	}
