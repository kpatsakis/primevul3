	void testNormalizeSyntaxMaskRequired() {
		TEST_ASSERT(testNormalizeMaskHelper(L"http://localhost/", URI_NORMALIZED));
		TEST_ASSERT(testNormalizeMaskHelper(L"httP://localhost/", URI_NORMALIZE_SCHEME));
		TEST_ASSERT(testNormalizeMaskHelper(L"http://%0d@localhost/", URI_NORMALIZE_USER_INFO));
		TEST_ASSERT(testNormalizeMaskHelper(L"http://localhosT/", URI_NORMALIZE_HOST));
		TEST_ASSERT(testNormalizeMaskHelper(L"http://localhost/./abc", URI_NORMALIZE_PATH));
		TEST_ASSERT(testNormalizeMaskHelper(L"http://localhost/?AB%43", URI_NORMALIZE_QUERY));
		TEST_ASSERT(testNormalizeMaskHelper(L"http://localhost/#AB%43", URI_NORMALIZE_FRAGMENT));
	}
