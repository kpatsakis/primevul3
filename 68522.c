check_compat_entry_size_and_hooks(struct compat_arpt_entry *e,
				  struct xt_table_info *newinfo,
				  unsigned int *size,
				  const unsigned char *base,
				  const unsigned char *limit)
{
	struct xt_entry_target *t;
	struct xt_target *target;
	unsigned int entry_offset;
	int ret, off;

	if ((unsigned long)e % __alignof__(struct compat_arpt_entry) != 0 ||
	    (unsigned char *)e + sizeof(struct compat_arpt_entry) >= limit ||
	    (unsigned char *)e + e->next_offset > limit)
		return -EINVAL;

	if (e->next_offset < sizeof(struct compat_arpt_entry) +
			     sizeof(struct compat_xt_entry_target))
		return -EINVAL;

	if (!arp_checkentry(&e->arp))
		return -EINVAL;

	ret = xt_compat_check_entry_offsets(e, e->elems, e->target_offset,
					    e->next_offset);
	if (ret)
		return ret;

	off = sizeof(struct arpt_entry) - sizeof(struct compat_arpt_entry);
	entry_offset = (void *)e - (void *)base;

	t = compat_arpt_get_target(e);
	target = xt_request_find_target(NFPROTO_ARP, t->u.user.name,
					t->u.user.revision);
	if (IS_ERR(target)) {
		ret = PTR_ERR(target);
		goto out;
	}
	t->u.kernel.target = target;

	off += xt_compat_target_offset(target);
	*size += off;
	ret = xt_compat_add_offset(NFPROTO_ARP, entry_offset, off);
	if (ret)
		goto release_target;

	return 0;

release_target:
	module_put(t->u.kernel.target->me);
out:
	return ret;
}
