archive_acl_add_entry(struct archive_acl *acl,
    int type, int permset, int tag, int id, const char *name)
{
	struct archive_acl_entry *ap;

	if (acl_special(acl, type, permset, tag) == 0)
		return ARCHIVE_OK;
	ap = acl_new_entry(acl, type, permset, tag, id);
	if (ap == NULL) {
		/* XXX Error XXX */
		return ARCHIVE_FAILED;
	}
	if (name != NULL  &&  *name != '\0')
		archive_mstring_copy_mbs(&ap->name, name);
	else
		archive_mstring_clean(&ap->name);
	return ARCHIVE_OK;
}
