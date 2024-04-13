static struct rb_entry *__lookup_rb_tree_fast(struct rb_entry *cached_re,
							unsigned int ofs)
{
	if (cached_re) {
		if (cached_re->ofs <= ofs &&
				cached_re->ofs + cached_re->len > ofs) {
			return cached_re;
		}
	}
	return NULL;
}
