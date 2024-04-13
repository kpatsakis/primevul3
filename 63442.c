	void testNormalizeSyntaxComponents() {
		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"http://%41@EXAMPLE.ORG/../a?%41#%41",
				URI_NORMALIZE_SCHEME));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"HTTP://A@EXAMPLE.ORG/../a?%41#%41",
				URI_NORMALIZE_USER_INFO));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"HTTP://%41@example.org/../a?%41#%41",
				URI_NORMALIZE_HOST));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"HTTP://%41@EXAMPLE.ORG/a?%41#%41",
				URI_NORMALIZE_PATH));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"HTTP://%41@EXAMPLE.ORG/../a?A#%41",
				URI_NORMALIZE_QUERY));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#%41",
				L"HTTP://%41@EXAMPLE.ORG/../a?%41#A",
				URI_NORMALIZE_FRAGMENT));
	}
