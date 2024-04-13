append_entry_w(wchar_t **wp, const wchar_t *prefix, int type,
    int tag, int flags, const wchar_t *wname, int perm, int id)
{
	int i;

	if (prefix != NULL) {
		wcscpy(*wp, prefix);
		*wp += wcslen(*wp);
	}
	switch (tag) {
	case ARCHIVE_ENTRY_ACL_USER_OBJ:
		wname = NULL;
		id = -1;
		if ((type & ARCHIVE_ENTRY_ACL_TYPE_NFS4) != 0) {
			wcscpy(*wp, L"owner@");
			break;
		}
		/* FALLTHROUGH */
	case ARCHIVE_ENTRY_ACL_USER:
		wcscpy(*wp, L"user");
		break;
	case ARCHIVE_ENTRY_ACL_GROUP_OBJ:
		wname = NULL;
		id = -1;
		if ((type & ARCHIVE_ENTRY_ACL_TYPE_NFS4) != 0) {
			wcscpy(*wp, L"group@");
			break;
		}
		/* FALLTHROUGH */
	case ARCHIVE_ENTRY_ACL_GROUP:
		wcscpy(*wp, L"group");
		break;
	case ARCHIVE_ENTRY_ACL_MASK:
		wcscpy(*wp, L"mask");
		wname = NULL;
		id = -1;
		break;
	case ARCHIVE_ENTRY_ACL_OTHER:
		wcscpy(*wp, L"other");
		wname = NULL;
		id = -1;
		break;
	case ARCHIVE_ENTRY_ACL_EVERYONE:
		wcscpy(*wp, L"everyone@");
		wname = NULL;
		id = -1;
		break;
	}
	*wp += wcslen(*wp);
	*(*wp)++ = L':';
	if (((type & ARCHIVE_ENTRY_ACL_TYPE_POSIX1E) != 0) ||
	    tag == ARCHIVE_ENTRY_ACL_USER ||
	    tag == ARCHIVE_ENTRY_ACL_GROUP) {
		if (wname != NULL) {
			wcscpy(*wp, wname);
			*wp += wcslen(*wp);
		} else if (tag == ARCHIVE_ENTRY_ACL_USER
		    || tag == ARCHIVE_ENTRY_ACL_GROUP) {
			append_id_w(wp, id);
			if ((type & ARCHIVE_ENTRY_ACL_TYPE_NFS4) == 0)
				id = -1;
		}
		/* Solaris style has no second colon after other and mask */
		if (((flags & ARCHIVE_ENTRY_ACL_STYLE_SOLARIS) == 0)
		    || (tag != ARCHIVE_ENTRY_ACL_OTHER
		    && tag != ARCHIVE_ENTRY_ACL_MASK))
			*(*wp)++ = L':';
	}
	if ((type & ARCHIVE_ENTRY_ACL_TYPE_POSIX1E) != 0) {
		/* POSIX.1e ACL perms */
		*(*wp)++ = (perm & 0444) ? L'r' : L'-';
		*(*wp)++ = (perm & 0222) ? L'w' : L'-';
		*(*wp)++ = (perm & 0111) ? L'x' : L'-';
	} else {
		/* NFSv4 ACL perms */
		for (i = 0; i < nfsv4_acl_perm_map_size; i++) {
			if (perm & nfsv4_acl_perm_map[i].perm)
				*(*wp)++ = nfsv4_acl_perm_map[i].wc;
			else if ((flags & ARCHIVE_ENTRY_ACL_STYLE_COMPACT) == 0)
				*(*wp)++ = L'-';
		}
		*(*wp)++ = L':';
		for (i = 0; i < nfsv4_acl_flag_map_size; i++) {
			if (perm & nfsv4_acl_flag_map[i].perm)
				*(*wp)++ = nfsv4_acl_flag_map[i].wc;
			else if ((flags & ARCHIVE_ENTRY_ACL_STYLE_COMPACT) == 0)
				*(*wp)++ = L'-';
		}
		*(*wp)++ = L':';
		switch (type) {
		case ARCHIVE_ENTRY_ACL_TYPE_ALLOW:
			wcscpy(*wp, L"allow");
			break;
		case ARCHIVE_ENTRY_ACL_TYPE_DENY:
			wcscpy(*wp, L"deny");
			break;
		case ARCHIVE_ENTRY_ACL_TYPE_AUDIT:
			wcscpy(*wp, L"audit");
			break;
		case ARCHIVE_ENTRY_ACL_TYPE_ALARM:
			wcscpy(*wp, L"alarm");
			break;
		default:
			break;
		}
		*wp += wcslen(*wp);
	}
	if (id != -1) {
		*(*wp)++ = L':';
		append_id_w(wp, id);
	}
}
