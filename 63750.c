static int report(struct fsck_options *options, struct object *object,
	enum fsck_msg_id id, const char *fmt, ...)
{
	va_list ap;
	struct strbuf sb = STRBUF_INIT;
	int msg_type = fsck_msg_type(id, options), result;

	if (msg_type == FSCK_IGNORE)
		return 0;

	if (options->skiplist && object &&
			oid_array_lookup(options->skiplist, &object->oid) >= 0)
		return 0;

	if (msg_type == FSCK_FATAL)
		msg_type = FSCK_ERROR;
	else if (msg_type == FSCK_INFO)
		msg_type = FSCK_WARN;

	append_msg_id(&sb, msg_id_info[id].id_string);

	va_start(ap, fmt);
	strbuf_vaddf(&sb, fmt, ap);
	result = options->error_func(options, object, msg_type, sb.buf);
	strbuf_release(&sb);
	va_end(ap);

	return result;
}
