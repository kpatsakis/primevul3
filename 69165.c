struct rb_entry *__lookup_rb_tree_ret(struct rb_root *root,
				struct rb_entry *cached_re,
				unsigned int ofs,
				struct rb_entry **prev_entry,
				struct rb_entry **next_entry,
				struct rb_node ***insert_p,
				struct rb_node **insert_parent,
				bool force)
{
	struct rb_node **pnode = &root->rb_node;
	struct rb_node *parent = NULL, *tmp_node;
	struct rb_entry *re = cached_re;

	*insert_p = NULL;
	*insert_parent = NULL;
	*prev_entry = NULL;
	*next_entry = NULL;

	if (RB_EMPTY_ROOT(root))
		return NULL;

	if (re) {
		if (re->ofs <= ofs && re->ofs + re->len > ofs)
			goto lookup_neighbors;
	}

	while (*pnode) {
		parent = *pnode;
		re = rb_entry(*pnode, struct rb_entry, rb_node);

		if (ofs < re->ofs)
			pnode = &(*pnode)->rb_left;
		else if (ofs >= re->ofs + re->len)
			pnode = &(*pnode)->rb_right;
		else
			goto lookup_neighbors;
	}

	*insert_p = pnode;
	*insert_parent = parent;

	re = rb_entry(parent, struct rb_entry, rb_node);
	tmp_node = parent;
	if (parent && ofs > re->ofs)
		tmp_node = rb_next(parent);
	*next_entry = rb_entry_safe(tmp_node, struct rb_entry, rb_node);

	tmp_node = parent;
	if (parent && ofs < re->ofs)
		tmp_node = rb_prev(parent);
	*prev_entry = rb_entry_safe(tmp_node, struct rb_entry, rb_node);
	return NULL;

lookup_neighbors:
	if (ofs == re->ofs || force) {
		/* lookup prev node for merging backward later */
		tmp_node = rb_prev(&re->rb_node);
		*prev_entry = rb_entry_safe(tmp_node, struct rb_entry, rb_node);
	}
	if (ofs == re->ofs + re->len - 1 || force) {
		/* lookup next node for merging frontward later */
		tmp_node = rb_next(&re->rb_node);
		*next_entry = rb_entry_safe(tmp_node, struct rb_entry, rb_node);
	}
	return re;
}
