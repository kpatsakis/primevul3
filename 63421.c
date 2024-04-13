int URI_FUNC(ComposeQueryEx)(URI_CHAR * dest,
		const URI_TYPE(QueryList) * queryList, int maxChars, int * charsWritten,
		UriBool spaceToPlus, UriBool normalizeBreaks) {
	if ((dest == NULL) || (queryList == NULL)) {
		return URI_ERROR_NULL;
	}

	if (maxChars < 1) {
		return URI_ERROR_OUTPUT_TOO_LARGE;
	}

	return URI_FUNC(ComposeQueryEngine)(dest, queryList, maxChars,
			charsWritten, NULL, spaceToPlus, normalizeBreaks);
}
