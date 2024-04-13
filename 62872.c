archive_acl_copy(struct archive_acl *dest, struct archive_acl *src)
{
	struct archive_acl_entry *ap, *ap2;

	archive_acl_clear(dest);

	dest->mode = src->mode;
	ap = src->acl_head;
	while (ap != NULL) {
		ap2 = acl_new_entry(dest,
		    ap->type, ap->permset, ap->tag, ap->id);
		if (ap2 != NULL)
			archive_mstring_copy(&ap2->name, &ap->name);
		ap = ap->next;
	}
}
