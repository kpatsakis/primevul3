	void testQueryStringEndingInEqualSign_NonBug32() {
		const char * queryString = "firstname=sdsd&lastname=";

		UriQueryListA * queryList = NULL;
		int itemCount = 0;
		const int res = uriDissectQueryMallocA(&queryList, &itemCount,
				queryString, queryString + strlen(queryString));

		TEST_ASSERT(res == URI_SUCCESS);
		TEST_ASSERT(itemCount == 2);
		TEST_ASSERT(queryList != NULL);
		TEST_ASSERT(strcmp(queryList->key, "firstname") == 0);
		TEST_ASSERT(strcmp(queryList->value, "sdsd") == 0);
		TEST_ASSERT(strcmp(queryList->next->key, "lastname") == 0);
		TEST_ASSERT(strcmp(queryList->next->value, "") == 0);
		TEST_ASSERT(queryList->next->next == NULL);

		uriFreeQueryListA(queryList);
	}
