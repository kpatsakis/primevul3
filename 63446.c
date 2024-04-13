	void testPervertedQueryString() {
		helperTestQueryString("http://example.org/?&&=&&&=&&&&==&===&====", 5);
	}
