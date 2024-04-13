static int canonicalize_directory_path(
	git_index *index,
	git_index_entry *entry,
	git_index_entry *existing)
{
	const git_index_entry *match, *best = NULL;
	char *search, *sep;
	size_t pos, search_len, best_len;

	if (!index->ignore_case)
		return 0;

	/* item already exists in the index, simply re-use the existing case */
	if (existing) {
		memcpy((char *)entry->path, existing->path, strlen(existing->path));
		return 0;
	}

	/* nothing to do */
	if (strchr(entry->path, '/') == NULL)
		return 0;

	if ((search = git__strdup(entry->path)) == NULL)
		return -1;

	/* starting at the parent directory and descending to the root, find the
	 * common parent directory.
	 */
	while (!best && (sep = strrchr(search, '/'))) {
		sep[1] = '\0';

		search_len = strlen(search);

		git_vector_bsearch2(
			&pos, &index->entries, index->entries_search_path, search);

		while ((match = git_vector_get(&index->entries, pos))) {
			if (GIT_IDXENTRY_STAGE(match) != 0) {
				/* conflicts do not contribute to canonical paths */
			} else if (strncmp(search, match->path, search_len) == 0) {
				/* prefer an exact match to the input filename */
				best = match;
				best_len = search_len;
				break;
			} else if (strncasecmp(search, match->path, search_len) == 0) {
				/* continue walking, there may be a path with an exact
				 * (case sensitive) match later in the index, but use this
				 * as the best match until that happens.
				 */
				if (!best) {
					best = match;
					best_len = search_len;
				}
			} else {
				break;
			}

			pos++;
		}

		sep[0] = '\0';
	}

	if (best)
		memcpy((char *)entry->path, best->path, best_len);

	git__free(search);
	return 0;
}
