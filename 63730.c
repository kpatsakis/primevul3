static const char *describe_object(struct fsck_options *o, struct object *obj)
{
	static struct strbuf buf = STRBUF_INIT;
	char *name;

	strbuf_reset(&buf);
	strbuf_addstr(&buf, oid_to_hex(&obj->oid));
	if (o->object_names && (name = lookup_decoration(o->object_names, obj)))
		strbuf_addf(&buf, " (%s)", name);

	return buf.buf;
}
