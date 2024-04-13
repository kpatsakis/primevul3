static void append_msg_id(struct strbuf *sb, const char *msg_id)
{
	for (;;) {
		char c = *(msg_id)++;

		if (!c)
			break;
		if (c != '_')
			strbuf_addch(sb, tolower(c));
		else {
			assert(*msg_id);
			strbuf_addch(sb, *(msg_id)++);
		}
	}

	strbuf_addstr(sb, ": ");
}
