	void testQueryListHelper(const wchar_t * input, int expectedItemCount) {
		int res;

		UriBool spacePlusConversion = URI_TRUE;
		UriBool normalizeBreaks = URI_FALSE;
		UriBreakConversion breakConversion = URI_BR_DONT_TOUCH;

		int itemCount;
		UriQueryListW * queryList;
		res = uriDissectQueryMallocExW(&queryList, &itemCount,
				input, input + wcslen(input), spacePlusConversion, breakConversion);
		TEST_ASSERT(res == URI_SUCCESS);
		TEST_ASSERT(itemCount == expectedItemCount);
		TEST_ASSERT((queryList == NULL) == (expectedItemCount == 0));

		if (expectedItemCount != 0) {
			int charsRequired;
			res = uriComposeQueryCharsRequiredExW(queryList, &charsRequired, spacePlusConversion,
					normalizeBreaks);
			TEST_ASSERT(res == URI_SUCCESS);
			TEST_ASSERT(charsRequired >= (int)wcslen(input));

			wchar_t * recomposed = new wchar_t[charsRequired + 1];
			int charsWritten;
			res = uriComposeQueryExW(recomposed, queryList, charsRequired + 1,
					&charsWritten, spacePlusConversion, normalizeBreaks);
			TEST_ASSERT(res == URI_SUCCESS);
			TEST_ASSERT(charsWritten <= charsRequired);
			TEST_ASSERT(charsWritten == (int)wcslen(input) + 1);
			TEST_ASSERT(!wcscmp(input, recomposed));
			delete [] recomposed;

			recomposed = NULL;
			res = uriComposeQueryMallocW(&recomposed, queryList);
			TEST_ASSERT(res == URI_SUCCESS);
			TEST_ASSERT(recomposed != NULL);
			TEST_ASSERT(charsWritten == (int)wcslen(input) + 1);
			TEST_ASSERT(!wcscmp(input, recomposed));
			free(recomposed);
		}

		uriFreeQueryListW(queryList);
	}
