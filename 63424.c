	void testAddBase() {
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g:h", L"g:h"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g", L"http://a/b/c/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"./g", L"http://a/b/c/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g/", L"http://a/b/c/g/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"/g", L"http://a/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"//g", L"http://g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"?y", L"http://a/b/c/d;p?y"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g?y", L"http://a/b/c/g?y"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"#s", L"http://a/b/c/d;p?q#s"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g#s", L"http://a/b/c/g#s"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g?y#s", L"http://a/b/c/g?y#s"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L";x", L"http://a/b/c/;x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g;x", L"http://a/b/c/g;x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g;x?y#s", L"http://a/b/c/g;x?y#s"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"", L"http://a/b/c/d;p?q"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L".", L"http://a/b/c/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"./", L"http://a/b/c/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"..", L"http://a/b/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../", L"http://a/b/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../g", L"http://a/b/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../..", L"http://a/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../../", L"http://a/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../../g", L"http://a/g"));

		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../../../g", L"http://a/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"../../../../g", L"http://a/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"/./g", L"http://a/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"/../g", L"http://a/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g.", L"http://a/b/c/g."));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L".g", L"http://a/b/c/.g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g..", L"http://a/b/c/g.."));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"..g", L"http://a/b/c/..g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"./../g", L"http://a/b/g"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"./g/.", L"http://a/b/c/g/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g/./h", L"http://a/b/c/g/h"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g/../h", L"http://a/b/c/h"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g;x=1/./y", L"http://a/b/c/g;x=1/y"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g;x=1/../y", L"http://a/b/c/y"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g?y/./x", L"http://a/b/c/g?y/./x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g?y/../x", L"http://a/b/c/g?y/../x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g#s/./x", L"http://a/b/c/g#s/./x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"g#s/../x", L"http://a/b/c/g#s/../x"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"http:g", L"http:g"));

		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"http:g", L"http:g", false));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"http:g", L"http://a/b/c/g", true));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"http:g?q#f", L"http://a/b/c/g?q#f", true));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"other:g?q#f", L"other:g?q#f", true));

		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"/", L"http://a/"));
		TEST_ASSERT(testAddBaseHelper(L"http://a/b/c/d;p?q", L"/g/", L"http://a/g/"));
	}
