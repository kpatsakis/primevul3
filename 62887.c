ismode_w(const wchar_t *start, const wchar_t *end, int *permset)
{
	const wchar_t *p;

	if (start >= end)
		return (0);
	p = start;
	*permset = 0;
	while (p < end) {
		switch (*p++) {
		case L'r': case L'R':
			*permset |= ARCHIVE_ENTRY_ACL_READ;
			break;
		case L'w': case L'W':
			*permset |= ARCHIVE_ENTRY_ACL_WRITE;
			break;
		case L'x': case L'X':
			*permset |= ARCHIVE_ENTRY_ACL_EXECUTE;
			break;
		case L'-':
			break;
		default:
			return (0);
		}
	}
	return (1);
}
