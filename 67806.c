static int write_disk_entry(git_filebuf *file, git_index_entry *entry, const char *last)
{
	void *mem = NULL;
	struct entry_short *ondisk;
	size_t path_len, disk_size;
	int varint_len = 0;
	char *path;
	const char *path_start = entry->path;
	size_t same_len = 0;

	path_len = ((struct entry_internal *)entry)->pathlen;

	if (last) {
		const char *last_c = last;

		while (*path_start == *last_c) {
			if (!*path_start || !*last_c)
				break;
			++path_start;
			++last_c;
			++same_len;
		}
		path_len -= same_len;
		varint_len = git_encode_varint(NULL, 0, same_len);
	}

	disk_size = index_entry_size(path_len, varint_len, entry->flags);

	if (git_filebuf_reserve(file, &mem, disk_size) < 0)
		return -1;

	ondisk = (struct entry_short *)mem;

	memset(ondisk, 0x0, disk_size);

	/**
	 * Yes, we have to truncate.
	 *
	 * The on-disk format for Index entries clearly defines
	 * the time and size fields to be 4 bytes each -- so even if
	 * we store these values with 8 bytes on-memory, they must
	 * be truncated to 4 bytes before writing to disk.
	 *
	 * In 2038 I will be either too dead or too rich to care about this
	 */
	ondisk->ctime.seconds = htonl((uint32_t)entry->ctime.seconds);
	ondisk->mtime.seconds = htonl((uint32_t)entry->mtime.seconds);
	ondisk->ctime.nanoseconds = htonl(entry->ctime.nanoseconds);
	ondisk->mtime.nanoseconds = htonl(entry->mtime.nanoseconds);
	ondisk->dev = htonl(entry->dev);
	ondisk->ino = htonl(entry->ino);
	ondisk->mode = htonl(entry->mode);
	ondisk->uid = htonl(entry->uid);
	ondisk->gid = htonl(entry->gid);
	ondisk->file_size = htonl((uint32_t)entry->file_size);

	git_oid_cpy(&ondisk->oid, &entry->id);

	ondisk->flags = htons(entry->flags);

	if (entry->flags & GIT_IDXENTRY_EXTENDED) {
		struct entry_long *ondisk_ext;
		ondisk_ext = (struct entry_long *)ondisk;
		ondisk_ext->flags_extended = htons(entry->flags_extended &
			GIT_IDXENTRY_EXTENDED_FLAGS);
		path = ondisk_ext->path;
		disk_size -= offsetof(struct entry_long, path);
	} else {
		path = ondisk->path;
		disk_size -= offsetof(struct entry_short, path);
	}

	if (last) {
		varint_len = git_encode_varint((unsigned char *) path,
					  disk_size, same_len);
		assert(varint_len > 0);
		path += varint_len;
		disk_size -= varint_len;

		/*
		 * If using path compression, we are not allowed
		 * to have additional trailing NULs.
		 */
		assert(disk_size == path_len + 1);
	} else {
		/*
		 * If no path compression is used, we do have
		 * NULs as padding. As such, simply assert that
		 * we have enough space left to write the path.
		 */
		assert(disk_size > path_len);
	}

	memcpy(path, path_start, path_len + 1);

	return 0;
}
