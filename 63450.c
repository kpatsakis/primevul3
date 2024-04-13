	void testQueryListPairHelper(const char * pair, const char * unescapedKey,
			const char * unescapedValue, const char * fixed = NULL) {
		int res;
		UriQueryListA * queryList;
		int itemCount;

		res = uriDissectQueryMallocA(&queryList, &itemCount, pair, pair + strlen(pair));
		TEST_ASSERT(res == URI_SUCCESS);
		TEST_ASSERT(queryList != NULL);
		TEST_ASSERT(itemCount == 1);
		TEST_ASSERT(!strcmp(queryList->key, unescapedKey));
		TEST_ASSERT(!strcmp(queryList->value, unescapedValue));

		char * recomposed;
		res = uriComposeQueryMallocA(&recomposed, queryList);
		TEST_ASSERT(res == URI_SUCCESS);
		TEST_ASSERT(recomposed != NULL);
		TEST_ASSERT(!strcmp(recomposed, (fixed != NULL) ? fixed : pair));
		free(recomposed);
		uriFreeQueryListA(queryList);
	}
