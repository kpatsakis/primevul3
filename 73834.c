static noinline enum slow_d_compare slow_dentry_cmp(
		const struct dentry *parent,
		struct dentry *dentry,
		unsigned int seq,
		const struct qstr *name)
{
	int tlen = dentry->d_name.len;
	const char *tname = dentry->d_name.name;

	if (read_seqcount_retry(&dentry->d_seq, seq)) {
		cpu_relax();
		return D_COMP_SEQRETRY;
	}
	if (parent->d_op->d_compare(parent, dentry, tlen, tname, name))
		return D_COMP_NOMATCH;
	return D_COMP_OK;
}
