static size_t index_entry_size(size_t path_len, size_t varint_len, uint32_t flags)
{
	if (varint_len) {
		if (flags & GIT_IDXENTRY_EXTENDED)
			return offsetof(struct entry_long, path) + path_len + 1 + varint_len;
		else
			return offsetof(struct entry_short, path) + path_len + 1 + varint_len;
	} else {
#define entry_size(type,len) ((offsetof(type, path) + (len) + 8) & ~7)
		if (flags & GIT_IDXENTRY_EXTENDED)
			return entry_size(struct entry_long, path_len);
		else
			return entry_size(struct entry_short, path_len);
#undef entry_size
 	}
 }
