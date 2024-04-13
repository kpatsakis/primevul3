append_entry(char **p, const char *prefix, int type,
    int tag, int flags, const char *name, int perm, int id)
{
	int i;

	if (prefix != NULL) {
		strcpy(*p, prefix);
		*p += strlen(*p);
	}
	switch (tag) {
	case ARCHIVE_ENTRY_ACL_USER_OBJ:
		name = NULL;
		id = -1;
		if ((type & ARCHIVE_ENTRY_ACL_TYPE_NFS4) != 0) {
			strcpy(*p, "owner@");
			break;
		}
		/* FALLTHROUGH */
	case ARCHIVE_ENTRY_ACL_USER:
		strcpy(*p, "user");
		break;
	case ARCHIVE_ENTRY_ACL_GROUP_OBJ:
		name = NULL;
		id = -1;
		if ((type & ARCHIVE_ENTRY_ACL_TYPE_NFS4) != 0) {
			strcpy(*p, "group@");
			break;
		}
		/* FALLTHROUGH */
	case ARCHIVE_ENTRY_ACL_GROUP:
		strcpy(*p, "group");
		break;
	case ARCHIVE_ENTRY_ACL_MASK:
		strcpy(*p, "mask");
		name = NULL;
		id = -1;
		break;
	case ARCHIVE_ENTRY_ACL_OTHER:
		strcpy(*p, "other");
		name = NULL;
		id = -1;
		break;
	case ARCHIVE_ENTRY_ACL_EVERYONE:
		strcpy(*p, "everyone@");
		name = NULL;
		id = -1;
		break;
	}
	*p += strlen(*p);
	*(*p)++ = ':';
	if (((type & ARCHIVE_ENTRY_ACL_TYPE_POSIX1E) != 0) ||
	    tag == ARCHIVE_ENTRY_ACL_USER ||
	    tag == ARCHIVE_ENTRY_ACL_GROUP) {
		if (name != NULL) {
			strcpy(*p, name);
			*p += strlen(*p);
		} else if (tag == ARCHIVE_ENTRY_ACL_USER
		    || tag == ARCHIVE_ENTRY_ACL_GROUP) {
			append_id(p, id);
			if ((type & ARCHIVE_ENTRY_ACL_TYPE_NFS4) == 0)
				id = -1;
		}
		/* Solaris style has no second colon after other and mask */
		if (((flags & ARCHIVE_ENTRY_ACL_STYLE_SOLARIS) == 0)
		    || (tag != ARCHIVE_ENTRY_ACL_OTHER
		    && tag != ARCHIVE_ENTRY_ACL_MASK))
			*(*p)++ = ':';
	}
	if ((type & ARCHIVE_ENTRY_ACL_TYPE_POSIX1E) != 0) {
		/* POSIX.1e ACL perms */
		*(*p)++ = (perm & 0444) ? 'r' : '-';
		*(*p)++ = (perm & 0222) ? 'w' : '-';
		*(*p)++ = (perm & 0111) ? 'x' : '-';
	} else {
		/* NFSv4 ACL perms */
		for (i = 0; i < nfsv4_acl_perm_map_size; i++) {
			if (perm & nfsv4_acl_perm_map[i].perm)
				*(*p)++ = nfsv4_acl_perm_map[i].c;
			else if ((flags & ARCHIVE_ENTRY_ACL_STYLE_COMPACT) == 0)
				*(*p)++ = '-';
		}
		*(*p)++ = ':';
		for (i = 0; i < nfsv4_acl_flag_map_size; i++) {
			if (perm & nfsv4_acl_flag_map[i].perm)
				*(*p)++ = nfsv4_acl_flag_map[i].c;
			else if ((flags & ARCHIVE_ENTRY_ACL_STYLE_COMPACT) == 0)
				*(*p)++ = '-';
		}
		*(*p)++ = ':';
		switch (type) {
		case ARCHIVE_ENTRY_ACL_TYPE_ALLOW:
			strcpy(*p, "allow");
			break;
		case ARCHIVE_ENTRY_ACL_TYPE_DENY:
			strcpy(*p, "deny");
			break;
		case ARCHIVE_ENTRY_ACL_TYPE_AUDIT:
			strcpy(*p, "audit");
			break;
		case ARCHIVE_ENTRY_ACL_TYPE_ALARM:
			strcpy(*p, "alarm");
			break;
		}
		*p += strlen(*p);
	}
	if (id != -1) {
		*(*p)++ = ':';
		append_id(p, id);
	}
}
