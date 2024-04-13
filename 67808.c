static int write_extension(git_filebuf *file, struct index_extension *header, git_buf *data)
{
	struct index_extension ondisk;

	memset(&ondisk, 0x0, sizeof(struct index_extension));
	memcpy(&ondisk, header, 4);
	ondisk.extension_size = htonl(header->extension_size);

	git_filebuf_write(file, &ondisk, sizeof(struct index_extension));
	return git_filebuf_write(file, data->ptr, data->size);
}
