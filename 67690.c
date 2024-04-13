GIT_INLINE(bool) is_file_or_link(const int filemode)
{
	return (filemode == GIT_FILEMODE_BLOB ||
		filemode == GIT_FILEMODE_BLOB_EXECUTABLE ||
		filemode == GIT_FILEMODE_LINK);
}
