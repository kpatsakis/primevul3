void git_index_entry__init_from_stat(
	git_index_entry *entry, struct stat *st, bool trust_mode)
{
	entry->ctime.seconds = (int32_t)st->st_ctime;
	entry->mtime.seconds = (int32_t)st->st_mtime;
#if defined(GIT_USE_NSEC)
	entry->mtime.nanoseconds = st->st_mtime_nsec;
	entry->ctime.nanoseconds = st->st_ctime_nsec;
#endif
	entry->dev  = st->st_rdev;
	entry->ino  = st->st_ino;
	entry->mode = (!trust_mode && S_ISREG(st->st_mode)) ?
		git_index__create_mode(0666) : git_index__create_mode(st->st_mode);
	entry->uid  = st->st_uid;
	entry->gid  = st->st_gid;
	entry->file_size = (uint32_t)st->st_size;
}
