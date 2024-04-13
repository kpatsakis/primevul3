archive_acl_add_entry_w_len(struct archive_acl *acl,
    int type, int permset, int tag, int id, const wchar_t *name, size_t len)
{
	struct archive_acl_entry *ap;

	if (acl_special(acl, type, permset, tag) == 0)
		return ARCHIVE_OK;
	ap = acl_new_entry(acl, type, permset, tag, id);
	if (ap == NULL) {
		/* XXX Error XXX */
		return ARCHIVE_FAILED;
	}
	if (name != NULL  &&  *name != L'\0' && len > 0)
		archive_mstring_copy_wcs_len(&ap->name, name, len);
	else
		archive_mstring_clean(&ap->name);
	return ARCHIVE_OK;
}
