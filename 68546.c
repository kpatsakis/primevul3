compat_copy_entry_from_user(struct compat_ipt_entry *e, void **dstptr,
			    unsigned int *size,
			    struct xt_table_info *newinfo, unsigned char *base)
{
	struct xt_entry_target *t;
	struct ipt_entry *de;
	unsigned int origsize;
	int h;
	struct xt_entry_match *ematch;

	origsize = *size;
	de = *dstptr;
	memcpy(de, e, sizeof(struct ipt_entry));
	memcpy(&de->counters, &e->counters, sizeof(e->counters));

	*dstptr += sizeof(struct ipt_entry);
	*size += sizeof(struct ipt_entry) - sizeof(struct compat_ipt_entry);

	xt_ematch_foreach(ematch, e)
		xt_compat_match_from_user(ematch, dstptr, size);

	de->target_offset = e->target_offset - (origsize - *size);
	t = compat_ipt_get_target(e);
	xt_compat_target_from_user(t, dstptr, size);

	de->next_offset = e->next_offset - (origsize - *size);

	for (h = 0; h < NF_INET_NUMHOOKS; h++) {
		if ((unsigned char *)de - base < newinfo->hook_entry[h])
			newinfo->hook_entry[h] -= origsize - *size;
		if ((unsigned char *)de - base < newinfo->underflow[h])
			newinfo->underflow[h] -= origsize - *size;
	}
}
