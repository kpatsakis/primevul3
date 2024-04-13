	bool testDistinctionHelper(const char * uriText, bool expectedHostSet,
			bool expectedAbsPath, bool expectedEmptyTailSegment) {
		UriParserStateA state;
		UriUriA uri;
		state.uri = &uri;

		int res = uriParseUriA(&state, uriText);
		if (res != URI_SUCCESS) {
			uriFreeUriMembersA(&uri);
			return false;
		}

		if (expectedHostSet != (uri.hostText.first != NULL)) {
			uriFreeUriMembersA(&uri);
			return false;
		}

		if (expectedAbsPath != (uri.absolutePath == URI_TRUE)) {
			uriFreeUriMembersA(&uri);
			return false;
		}

		if (expectedEmptyTailSegment != ((uri.pathTail != NULL)
				&& (uri.pathTail->text.first == uri.pathTail->text.afterLast))) {
			uriFreeUriMembersA(&uri);
			return false;
		}

		uriFreeUriMembersA(&uri);
		return true;
	}
