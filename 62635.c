struct dentry *ovl_entry_real(struct ovl_entry *oe, bool *is_upper)
{
	struct dentry *realdentry;

	realdentry = ovl_upperdentry_dereference(oe);
	if (realdentry) {
		*is_upper = true;
	} else {
		realdentry = oe->lowerdentry;
		*is_upper = false;
	}
	return realdentry;
}
