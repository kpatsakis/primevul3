static int compare_checksum(git_index *index)
{
	int fd;
	ssize_t bytes_read;
	git_oid checksum = {{ 0 }};

	if ((fd = p_open(index->index_file_path, O_RDONLY)) < 0)
		return fd;

	if (p_lseek(fd, -20, SEEK_END) < 0) {
		p_close(fd);
		giterr_set(GITERR_OS, "failed to seek to end of file");
		return -1;
	}

	bytes_read = p_read(fd, &checksum, GIT_OID_RAWSZ);
	p_close(fd);

	if (bytes_read < 0)
		return -1;

	return !!git_oid_cmp(&checksum, &index->checksum);
}
