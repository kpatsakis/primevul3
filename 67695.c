static int check_file_directory_collision(git_index *index,
		git_index_entry *entry, size_t pos, int ok_to_replace)
{
	int retval = has_file_name(index, entry, pos, ok_to_replace);
	retval = retval + has_dir_name(index, entry, ok_to_replace);

	if (retval) {
		giterr_set(GITERR_INDEX,
			"'%s' appears as both a file and a directory", entry->path);
		return -1;
	}

	return 0;
}
