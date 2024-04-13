archive_acl_count(struct archive_acl *acl, int want_type)
{
	int count;
	struct archive_acl_entry *ap;

	count = 0;
	ap = acl->acl_head;
	while (ap != NULL) {
		if ((ap->type & want_type) != 0)
			count++;
		ap = ap->next;
	}

	if (count > 0 && ((want_type & ARCHIVE_ENTRY_ACL_TYPE_ACCESS) != 0))
		count += 3;
	return (count);
}
