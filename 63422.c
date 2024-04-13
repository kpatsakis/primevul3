int URI_FUNC(ComposeQueryMalloc)(URI_CHAR ** dest,
		const URI_TYPE(QueryList) * queryList) {
	const UriBool spaceToPlus = URI_TRUE;
	const UriBool normalizeBreaks = URI_TRUE;

	return URI_FUNC(ComposeQueryMallocEx)(dest, queryList,
			spaceToPlus, normalizeBreaks);
}
