check_compat_entry_size_and_hooks(struct compat_ipt_entry *e,
				  struct xt_table_info *newinfo,
				  unsigned int *size,
				  const unsigned char *base,
				  const unsigned char *limit)
{
	struct xt_entry_match *ematch;
	struct xt_entry_target *t;
	struct xt_target *target;
	unsigned int entry_offset;
	unsigned int j;
	int ret, off;

	if ((unsigned long)e % __alignof__(struct compat_ipt_entry) != 0 ||
	    (unsigned char *)e + sizeof(struct compat_ipt_entry) >= limit ||
	    (unsigned char *)e + e->next_offset > limit)
		return -EINVAL;

	if (e->next_offset < sizeof(struct compat_ipt_entry) +
			     sizeof(struct compat_xt_entry_target))
		return -EINVAL;

	if (!ip_checkentry(&e->ip))
		return -EINVAL;

	ret = xt_compat_check_entry_offsets(e, e->elems,
					    e->target_offset, e->next_offset);
	if (ret)
		return ret;

	off = sizeof(struct ipt_entry) - sizeof(struct compat_ipt_entry);
	entry_offset = (void *)e - (void *)base;
	j = 0;
	xt_ematch_foreach(ematch, e) {
		ret = compat_find_calc_match(ematch, &e->ip, &off);
		if (ret != 0)
			goto release_matches;
		++j;
	}

	t = compat_ipt_get_target(e);
	target = xt_request_find_target(NFPROTO_IPV4, t->u.user.name,
					t->u.user.revision);
	if (IS_ERR(target)) {
		ret = PTR_ERR(target);
		goto release_matches;
	}
	t->u.kernel.target = target;

	off += xt_compat_target_offset(target);
	*size += off;
	ret = xt_compat_add_offset(AF_INET, entry_offset, off);
	if (ret)
		goto out;

	return 0;

out:
	module_put(t->u.kernel.target->me);
release_matches:
	xt_ematch_foreach(ematch, e) {
		if (j-- == 0)
			break;
		module_put(ematch->u.kernel.match->me);
	}
	return ret;
}
