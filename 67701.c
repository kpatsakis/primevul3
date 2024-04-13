unsigned int git_index__create_mode(unsigned int mode)
{
	if (S_ISLNK(mode))
		return S_IFLNK;

	if (S_ISDIR(mode) || (mode & S_IFMT) == (S_IFLNK | S_IFDIR))
		return (S_IFLNK | S_IFDIR);

	return S_IFREG | GIT_PERMS_CANONICAL(mode);
}
