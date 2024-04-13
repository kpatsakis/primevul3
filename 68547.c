compat_copy_entry_to_user(struct ipt_entry *e, void __user **dstptr,
			  unsigned int *size, struct xt_counters *counters,
			  unsigned int i)
{
	struct xt_entry_target *t;
	struct compat_ipt_entry __user *ce;
	u_int16_t target_offset, next_offset;
	compat_uint_t origsize;
	const struct xt_entry_match *ematch;
	int ret = 0;

	origsize = *size;
	ce = *dstptr;
	if (copy_to_user(ce, e, sizeof(struct ipt_entry)) != 0 ||
	    copy_to_user(&ce->counters, &counters[i],
	    sizeof(counters[i])) != 0)
		return -EFAULT;

	*dstptr += sizeof(struct compat_ipt_entry);
	*size -= sizeof(struct ipt_entry) - sizeof(struct compat_ipt_entry);

	xt_ematch_foreach(ematch, e) {
		ret = xt_compat_match_to_user(ematch, dstptr, size);
		if (ret != 0)
			return ret;
	}
	target_offset = e->target_offset - (origsize - *size);
	t = ipt_get_target(e);
	ret = xt_compat_target_to_user(t, dstptr, size);
	if (ret)
		return ret;
	next_offset = e->next_offset - (origsize - *size);
	if (put_user(target_offset, &ce->target_offset) != 0 ||
	    put_user(next_offset, &ce->next_offset) != 0)
		return -EFAULT;
	return 0;
}
