static int parse_msg_id(const char *text)
{
	int i;

	if (!msg_id_info[0].downcased) {
		/* convert id_string to lower case, without underscores. */
		for (i = 0; i < FSCK_MSG_MAX; i++) {
			const char *p = msg_id_info[i].id_string;
			int len = strlen(p);
			char *q = xmalloc(len);

			msg_id_info[i].downcased = q;
			while (*p)
				if (*p == '_')
					p++;
				else
					*(q)++ = tolower(*(p)++);
			*q = '\0';
		}
	}

	for (i = 0; i < FSCK_MSG_MAX; i++)
		if (!strcmp(text, msg_id_info[i].downcased))
			return i;

	return -1;
}
