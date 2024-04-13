static int fsck_tag(struct tag *tag, const char *data,
	unsigned long size, struct fsck_options *options)
{
	struct object *tagged = tag->tagged;

	if (!tagged)
		return report(options, &tag->object, FSCK_MSG_BAD_TAG_OBJECT, "could not load tagged object");

	return fsck_tag_buffer(tag, data, size, options);
}
