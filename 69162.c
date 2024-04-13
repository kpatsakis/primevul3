struct rb_entry *__lookup_rb_tree(struct rb_root *root,
				struct rb_entry *cached_re, unsigned int ofs)
{
	struct rb_entry *re;

	re = __lookup_rb_tree_fast(cached_re, ofs);
	if (!re)
		return __lookup_rb_tree_slow(root, ofs);

	return re;
}
