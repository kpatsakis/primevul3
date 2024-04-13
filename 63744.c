static int fsck_walk_tag(struct tag *tag, void *data, struct fsck_options *options)
{
	char *name = get_object_name(options, &tag->object);

	if (parse_tag(tag))
		return -1;
	if (name)
		put_object_name(options, tag->tagged, "%s", name);
	return options->walk(tag->tagged, OBJ_ANY, data, options);
}
