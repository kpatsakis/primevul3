static int write_index(git_oid *checksum, git_index *index, git_filebuf *file)
{
	git_oid hash_final;
	struct index_header header;
	bool is_extended;
	uint32_t index_version_number;

	assert(index && file);

	if (index->version <= INDEX_VERSION_NUMBER_EXT)  {
		is_extended = is_index_extended(index);
		index_version_number = is_extended ? INDEX_VERSION_NUMBER_EXT : INDEX_VERSION_NUMBER_LB;
	} else {
		index_version_number = index->version;
	}

	header.signature = htonl(INDEX_HEADER_SIG);
	header.version = htonl(index_version_number);
	header.entry_count = htonl((uint32_t)index->entries.length);

	if (git_filebuf_write(file, &header, sizeof(struct index_header)) < 0)
		return -1;

	if (write_entries(index, file) < 0)
		return -1;

	/* write the tree cache extension */
	if (index->tree != NULL && write_tree_extension(index, file) < 0)
		return -1;

	/* write the rename conflict extension */
	if (index->names.length > 0 && write_name_extension(index, file) < 0)
		return -1;

	/* write the reuc extension */
	if (index->reuc.length > 0 && write_reuc_extension(index, file) < 0)
		return -1;

	/* get out the hash for all the contents we've appended to the file */
	git_filebuf_hash(&hash_final, file);
	git_oid_cpy(checksum, &hash_final);

	/* write it at the end of the file */
	if (git_filebuf_write(file, hash_final.id, GIT_OID_RAWSZ) < 0)
		return -1;

	/* file entries are no longer up to date */
	clear_uptodate(index);

	return 0;
}
