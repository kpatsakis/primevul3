int URI_FUNC(ComposeQueryMallocEx)(URI_CHAR ** dest,
		const URI_TYPE(QueryList) * queryList,
		UriBool spaceToPlus, UriBool normalizeBreaks) {
	int charsRequired;
	int res;
	URI_CHAR * queryString;

	if (dest == NULL) {
		return URI_ERROR_NULL;
	}

	/* Calculate space */
	res = URI_FUNC(ComposeQueryCharsRequiredEx)(queryList, &charsRequired,
			spaceToPlus, normalizeBreaks);
	if (res != URI_SUCCESS) {
		return res;
	}
	charsRequired++;

	/* Allocate space */
	queryString = malloc(charsRequired * sizeof(URI_CHAR));
	if (queryString == NULL) {
		return URI_ERROR_MALLOC;
	}

	/* Put query in */
	res = URI_FUNC(ComposeQueryEx)(queryString, queryList, charsRequired,
			NULL, spaceToPlus, normalizeBreaks);
	if (res != URI_SUCCESS) {
		free(queryString);
		return res;
	}

	*dest = queryString;
	return URI_SUCCESS;
}
