static char *parse_title(const char *name, size_t *len)
{
	const char *escapes = "'\\";
	char *title;
	char *end;
	char *ch;

	if (*name != '\'') {
		*len = strcspn(name, "|");
		if (!*len)
			return NULL;
		return strndup(name, *len);
	}

	title = strdup(name + 1);
	if (!title)
		return NULL;

	*len = 1;
	ch = title;
	end = title + strlen(title);
	while (ch < end) {
		size_t l = strcspn(ch, escapes);
		*len += l + 1;
		ch += l;
		switch (*ch) {
		case '\'':
			*ch = 0;
			return title;
		case '\\':
			if (!ch[1] || strcspn(ch + 1, escapes)) {
				free(title);
				return NULL;
			}
			memmove(ch, ch + 1, strlen(ch));
			ch++;
			(*len)++;
			break;
		default:
			free(title);
			return NULL;
		}
	}

	free(title);
	return NULL;
}