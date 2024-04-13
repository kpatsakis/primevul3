static void index_entry_cpy_nocache(
	git_index_entry *tgt,
	const git_index_entry *src)
{
	git_oid_cpy(&tgt->id, &src->id);
	tgt->mode = src->mode;
	tgt->flags = src->flags;
	tgt->flags_extended = (src->flags_extended & GIT_IDXENTRY_EXTENDED_FLAGS);
}
