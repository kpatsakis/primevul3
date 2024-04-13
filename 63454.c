	void testToStringCharsRequired() {
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com/"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com:80/"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://user:pass@www.example.com/"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com/index.html"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com/?abc"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com/#def"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"http://www.example.com/?abc#def"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"/test"));
		TEST_ASSERT(testToStringCharsRequiredHelper(L"test"));
	}
