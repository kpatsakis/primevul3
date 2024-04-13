GIT_INLINE(bool) valid_filemode(const int filemode)
{
	return (is_file_or_link(filemode) || filemode == GIT_FILEMODE_COMMIT);
}
