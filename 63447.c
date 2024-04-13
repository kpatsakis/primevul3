	void testQueryDissection_Bug3590761() {
		int res;
		UriQueryListA * queryList;
		int itemCount;
		const char * const pair = "q=hello&x=&y=";

		res = uriDissectQueryMallocA(&queryList, &itemCount, pair, pair + strlen(pair));
		TEST_ASSERT(res == URI_SUCCESS);
		TEST_ASSERT(queryList != NULL);
		TEST_ASSERT(itemCount == 3);

		TEST_ASSERT(!strcmp(queryList->key, "q"));
		TEST_ASSERT(!strcmp(queryList->value, "hello"));

		TEST_ASSERT(!strcmp(queryList->next->key, "x"));
		TEST_ASSERT(!strcmp(queryList->next->value, ""));

		TEST_ASSERT(!strcmp(queryList->next->next->key, "y"));
		TEST_ASSERT(!strcmp(queryList->next->next->value, ""));

		TEST_ASSERT(! queryList->next->next->next);

 		uriFreeQueryListA(queryList);
 	}
