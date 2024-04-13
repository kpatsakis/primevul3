	void testHostTextTermination_Issue15() {
		UriParserStateA state;
		UriUriA uri;
		state.uri = &uri;

		const char * const emptyHostWithPortUri = "//:123";
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&state, emptyHostWithPortUri));
		TEST_ASSERT(uri.hostText.first == emptyHostWithPortUri + strlen("//"));
		TEST_ASSERT(uri.hostText.afterLast == uri.hostText.first + 0);
		TEST_ASSERT(uri.portText.first == emptyHostWithPortUri
															+ strlen("//:"));
		TEST_ASSERT(uri.portText.afterLast == uri.portText.first
															+ strlen("123"));
		uriFreeUriMembersA(&uri);

		const char * const hostWithPortUri = "//h:123";
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&state, hostWithPortUri));
		TEST_ASSERT(uri.hostText.first == hostWithPortUri + strlen("//"));
		TEST_ASSERT(uri.hostText.afterLast == uri.hostText.first
															+ strlen("h"));
		TEST_ASSERT(uri.portText.first == hostWithPortUri + strlen("//h:"));
		TEST_ASSERT(uri.portText.afterLast == uri.portText.first
															+ strlen("123"));
		uriFreeUriMembersA(&uri);

		const char * const emptyHostEmptyUserInfoUri = "//@";
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&state,
												emptyHostEmptyUserInfoUri));
		TEST_ASSERT(uri.userInfo.first == emptyHostEmptyUserInfoUri
															+ strlen("//"));
		TEST_ASSERT(uri.userInfo.afterLast == uri.userInfo.first + 0);
		TEST_ASSERT(uri.hostText.first == emptyHostEmptyUserInfoUri
															+ strlen("//@"));
		TEST_ASSERT(uri.hostText.afterLast == uri.hostText.first + 0);
		uriFreeUriMembersA(&uri);

		const char * const hostEmptyUserInfoUri = "//@h";
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&state, hostEmptyUserInfoUri));
		TEST_ASSERT(uri.userInfo.first == hostEmptyUserInfoUri + strlen("//"));
		TEST_ASSERT(uri.userInfo.afterLast == uri.userInfo.first + 0);
		TEST_ASSERT(uri.hostText.first == hostEmptyUserInfoUri
															+ strlen("//@"));
		TEST_ASSERT(uri.hostText.afterLast == uri.hostText.first
															+ strlen("h"));
		uriFreeUriMembersA(&uri);

		const char * const emptyHostWithUserInfoUri = "//:@";
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&state,
												emptyHostWithUserInfoUri));
		TEST_ASSERT(uri.userInfo.first == emptyHostWithUserInfoUri
															+ strlen("//"));
		TEST_ASSERT(uri.userInfo.afterLast == uri.userInfo.first + 1);
		TEST_ASSERT(uri.hostText.first == emptyHostWithUserInfoUri
															+ strlen("//:@"));
		TEST_ASSERT(uri.hostText.afterLast == uri.hostText.first + 0);
		uriFreeUriMembersA(&uri);

		const char * const issue15Uri = "//:%aa@";
		TEST_ASSERT(URI_SUCCESS == uriParseUriA(&state, issue15Uri));
		TEST_ASSERT(uri.userInfo.first == issue15Uri + strlen("//"));
		TEST_ASSERT(uri.userInfo.afterLast == uri.userInfo.first
															+ strlen(":%aa"));
		TEST_ASSERT(uri.hostText.first == issue15Uri + strlen("//:%aa@"));
		TEST_ASSERT(uri.hostText.afterLast == uri.hostText.first + 0);
		uriFreeUriMembersA(&uri);
	}
