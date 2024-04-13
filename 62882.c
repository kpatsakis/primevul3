is_nfs4_perms(const char *start, const char *end, int *permset)
{
	const char *p = start;

	while (p < end) {
		switch (*p++) {
		case 'r':
			*permset |= ARCHIVE_ENTRY_ACL_READ_DATA;
			break;
		case 'w':
			*permset |= ARCHIVE_ENTRY_ACL_WRITE_DATA;
			break;
		case 'x':
			*permset |= ARCHIVE_ENTRY_ACL_EXECUTE;
			break;
		case 'p':
			*permset |= ARCHIVE_ENTRY_ACL_APPEND_DATA;
			break;
		case 'D':
			*permset |= ARCHIVE_ENTRY_ACL_DELETE_CHILD;
			break;
		case 'd':
			*permset |= ARCHIVE_ENTRY_ACL_DELETE;
			break;
		case 'a':
			*permset |= ARCHIVE_ENTRY_ACL_READ_ATTRIBUTES;
			break;
		case 'A':
			*permset |= ARCHIVE_ENTRY_ACL_WRITE_ATTRIBUTES;
			break;
		case 'R':
			*permset |= ARCHIVE_ENTRY_ACL_READ_NAMED_ATTRS;
			break;
		case 'W':
			*permset |= ARCHIVE_ENTRY_ACL_WRITE_NAMED_ATTRS;
			break;
		case 'c':
			*permset |= ARCHIVE_ENTRY_ACL_READ_ACL;
			break;
		case 'C':
			*permset |= ARCHIVE_ENTRY_ACL_WRITE_ACL;
			break;
		case 'o':
			*permset |= ARCHIVE_ENTRY_ACL_WRITE_OWNER;
			break;
		case 's':
			*permset |= ARCHIVE_ENTRY_ACL_SYNCHRONIZE;
			break;
		case '-':
			break;
		default:
			return(0);
		}
	}
	return (1);
}
