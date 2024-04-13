void lha_file_header_free(LHAFileHeader *header)
{

	if (header->_refcount == 0) {
		return;
	}


	--header->_refcount;

	if (header->_refcount > 0) {
		return;
	}

	free(header->filename);
	free(header->path);
	free(header->symlink_target);
	free(header->unix_username);
	free(header->unix_group);
	free(header);
}
