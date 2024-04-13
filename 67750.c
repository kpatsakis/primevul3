int git_index_set_version(git_index *index, unsigned int version)
{
	assert(index);

	if (version < INDEX_VERSION_NUMBER_LB ||
	    version > INDEX_VERSION_NUMBER_UB) {
		giterr_set(GITERR_INDEX, "invalid version number");
		return -1;
	}

	index->version = version;

	return 0;
}
