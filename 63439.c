	void testIpSixFail() {
		URI_TEST_IP_SIX_FAIL("::12345");

		URI_TEST_IP_SIX_FAIL("abcd::abcd::abcd");

		URI_TEST_IP_SIX_FAIL(":::1234");
		URI_TEST_IP_SIX_FAIL("1234:::1234:1234");
		URI_TEST_IP_SIX_FAIL("1234:1234:::1234");
		URI_TEST_IP_SIX_FAIL("1234:::");

		URI_TEST_IP_SIX_FAIL("1.2.3.4");
		URI_TEST_IP_SIX_FAIL("0001.0002.0003.0004");

		URI_TEST_IP_SIX_FAIL("0000:0000:0000:0000:0000:1.2.3.4");

		URI_TEST_IP_SIX_FAIL("0:0:0:0:0:0:0");
		URI_TEST_IP_SIX_FAIL("0:0:0:0:0:0:0:");
		URI_TEST_IP_SIX_FAIL("0:0:0:0:0:0:0:1.2.3.4");

		URI_TEST_IP_SIX_FAIL("1:2:3:4:5:6:7:8:9");
		URI_TEST_IP_SIX_FAIL("::2:3:4:5:6:7:8:9");
		URI_TEST_IP_SIX_FAIL("1:2:3:4::6:7:8:9");
		URI_TEST_IP_SIX_FAIL("1:2:3:4:5:6:7:8::");

		URI_TEST_IP_SIX_FAIL("::ffff:001.02.03.004"); // Leading zeros
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3.1111"); // Four char octet
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3.256"); // > 255
		URI_TEST_IP_SIX_FAIL("::ffff:311.2.3.4"); // > 155
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3:4"); // Not a dot
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3"); // Missing octet
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3."); // Missing octet
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3a.4"); // Hex in octet
		URI_TEST_IP_SIX_FAIL("::ffff:1.2.3.4:123"); // Crap input

		URI_TEST_IP_SIX_FAIL("g:0:0:0:0:0:0");
	}
