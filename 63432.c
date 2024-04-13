	void testEscaping() {
		const bool SPACE_TO_PLUS = true;
		const bool SPACE_TO_PERCENT = false;
		const bool KEEP_UNMODIFIED = false;
		const bool NORMALIZE = true;

		TEST_ASSERT(testEscapingHelper(L"abc def", L"abc+def", SPACE_TO_PLUS));
		TEST_ASSERT(testEscapingHelper(L"abc def", L"abc%20def", SPACE_TO_PERCENT));

		TEST_ASSERT(testEscapingHelper(L"\x00", L"\0"));
		TEST_ASSERT(testEscapingHelper(L"\x01", L"%01"));
		TEST_ASSERT(testEscapingHelper(L"\xff", L"%FF"));

		TEST_ASSERT(testEscapingHelper(L"\x0d", L"%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"g\x0d", L"g%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0dg", L"%0D%0Ag", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0d", L"%0D", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"g\x0d", L"g%0D", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"\x0dg", L"%0Dg", SPACE_TO_PLUS, KEEP_UNMODIFIED));

		TEST_ASSERT(testEscapingHelper(L"\x0a", L"%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"g\x0a", L"g%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0ag", L"%0D%0Ag", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0a", L"%0A", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"g\x0a", L"g%0A", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"\x0ag", L"%0Ag", SPACE_TO_PLUS, KEEP_UNMODIFIED));

		TEST_ASSERT(testEscapingHelper(L"\x0d\x0a", L"%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"g\x0d\x0a", L"g%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0d\x0ag", L"%0D%0Ag", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0d\x0a", L"%0D%0A", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"g\x0d\x0a", L"g%0D%0A", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"\x0d\x0ag", L"%0D%0Ag", SPACE_TO_PLUS, KEEP_UNMODIFIED));

		TEST_ASSERT(testEscapingHelper(L"\x0a\x0d", L"%0D%0A%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"g\x0a\x0d", L"g%0D%0A%0D%0A", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0a\x0dg", L"%0D%0A%0D%0Ag", SPACE_TO_PLUS, NORMALIZE));
		TEST_ASSERT(testEscapingHelper(L"\x0a\x0d", L"%0A%0D", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"g\x0a\x0d", L"g%0A%0D", SPACE_TO_PLUS, KEEP_UNMODIFIED));
		TEST_ASSERT(testEscapingHelper(L"\x0a\x0dg", L"%0A%0Dg", SPACE_TO_PLUS, KEEP_UNMODIFIED));
	}
