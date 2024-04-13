acl_special(struct archive_acl *acl, int type, int permset, int tag)
{
	if (type == ARCHIVE_ENTRY_ACL_TYPE_ACCESS
	    && ((permset & ~007) == 0)) {
		switch (tag) {
		case ARCHIVE_ENTRY_ACL_USER_OBJ:
			acl->mode &= ~0700;
			acl->mode |= (permset & 7) << 6;
			return (0);
		case ARCHIVE_ENTRY_ACL_GROUP_OBJ:
			acl->mode &= ~0070;
			acl->mode |= (permset & 7) << 3;
			return (0);
		case ARCHIVE_ENTRY_ACL_OTHER:
			acl->mode &= ~0007;
			acl->mode |= permset & 7;
			return (0);
		}
	}
	return (1);
}
