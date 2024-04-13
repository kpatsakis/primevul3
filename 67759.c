void git_indexwriter_cleanup(git_indexwriter *writer)
{
	git_filebuf_cleanup(&writer->file);

	git_index_free(writer->index);
	writer->index = NULL;
}
