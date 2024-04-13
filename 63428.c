	void testDistinction() {
		/*
============================================================================
Rule                                | Example | hostSet | absPath | emptySeg
------------------------------------|---------|---------|---------|---------
1) URI = scheme ":" hier-part ...   |         |         |         |
   1) "//" authority path-abempty   | "s://"  | true    |   false |   false
                                    | "s:///" | true    |   false | true
                                    | "s://a" | true    |   false |   false
                                    | "s://a/"| true    |   false | true
   2) path-absolute                 | "s:/"   |   false | true    |   false
   3) path-rootless                 | "s:a"   |   false |   false |   false
                                    | "s:a/"  |   false |   false | true
   4) path-empty                    | "s:"    |   false |   false |   false
------------------------------------|---------|---------|---------|---------
2) relative-ref = relative-part ... |         |         |         |
   1) "//" authority path-abempty   | "//"    | true    |   false |   false
                                    | "///"   | true    |   false | true
   2) path-absolute                 | "/"     |   false | true    |   false
   3) path-noscheme                 | "a"     |   false |   false |   false
                                    | "a/"    |   false |   false | true
   4) path-empty                    | ""      |   false |   false |   false
============================================================================
		*/
		TEST_ASSERT(testDistinctionHelper("s://", true, false, false));
		TEST_ASSERT(testDistinctionHelper("s:///", true, false, true));
		TEST_ASSERT(testDistinctionHelper("s://a", true, false, false));
		TEST_ASSERT(testDistinctionHelper("s://a/", true, false, true));
		TEST_ASSERT(testDistinctionHelper("s:/", false, true, false));
		TEST_ASSERT(testDistinctionHelper("s:a", false, false, false));
		TEST_ASSERT(testDistinctionHelper("s:a/", false, false, true));
		TEST_ASSERT(testDistinctionHelper("s:", false, false, false));

		TEST_ASSERT(testDistinctionHelper("//", true, false, false));
		TEST_ASSERT(testDistinctionHelper("///", true, false, true));
		TEST_ASSERT(testDistinctionHelper("/", false, true, false));
		TEST_ASSERT(testDistinctionHelper("a", false, false, false));
		TEST_ASSERT(testDistinctionHelper("a/", false, false, true));
		TEST_ASSERT(testDistinctionHelper("", false, false, false));
	}
