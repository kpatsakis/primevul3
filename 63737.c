int fsck_object(struct object *obj, void *data, unsigned long size,
	struct fsck_options *options)
{
	if (!obj)
		return report(options, obj, FSCK_MSG_BAD_OBJECT_SHA1, "no valid object to fsck");

	if (obj->type == OBJ_BLOB)
		return fsck_blob((struct blob *)obj, data, size, options);
	if (obj->type == OBJ_TREE)
		return fsck_tree((struct tree *) obj, options);
	if (obj->type == OBJ_COMMIT)
		return fsck_commit((struct commit *) obj, (const char *) data,
			size, options);
	if (obj->type == OBJ_TAG)
		return fsck_tag((struct tag *) obj, (const char *) data,
			size, options);

	return report(options, obj, FSCK_MSG_UNKNOWN_TYPE, "unknown type '%d' (internal fsck error)",
			  obj->type);
}
