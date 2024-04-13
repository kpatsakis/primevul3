static void put_object_name(struct fsck_options *options, struct object *obj,
	const char *fmt, ...)
{
	va_list ap;
	struct strbuf buf = STRBUF_INIT;
	char *existing;

	if (!options->object_names)
		return;
	existing = lookup_decoration(options->object_names, obj);
	if (existing)
		return;
	va_start(ap, fmt);
	strbuf_vaddf(&buf, fmt, ap);
	add_decoration(options->object_names, obj, strbuf_detach(&buf, NULL));
	va_end(ap);
}
