static int compat_copy_entry_to_user(struct arpt_entry *e, void __user **dstptr,
				     compat_uint_t *size,
				     struct xt_counters *counters,
				     unsigned int i)
{
	struct xt_entry_target *t;
	struct compat_arpt_entry __user *ce;
	u_int16_t target_offset, next_offset;
	compat_uint_t origsize;
	int ret;

	origsize = *size;
	ce = *dstptr;
	if (copy_to_user(ce, e, sizeof(struct arpt_entry)) != 0 ||
	    copy_to_user(&ce->counters, &counters[i],
	    sizeof(counters[i])) != 0)
		return -EFAULT;

	*dstptr += sizeof(struct compat_arpt_entry);
	*size -= sizeof(struct arpt_entry) - sizeof(struct compat_arpt_entry);

	target_offset = e->target_offset - (origsize - *size);

	t = arpt_get_target(e);
	ret = xt_compat_target_to_user(t, dstptr, size);
	if (ret)
		return ret;
	next_offset = e->next_offset - (origsize - *size);
	if (put_user(target_offset, &ce->target_offset) != 0 ||
	    put_user(next_offset, &ce->next_offset) != 0)
		return -EFAULT;
	return 0;
}
