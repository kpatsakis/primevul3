static int fsck_walk_tree(struct tree *tree, void *data, struct fsck_options *options)
{
	struct tree_desc desc;
	struct name_entry entry;
	int res = 0;
	const char *name;

	if (parse_tree(tree))
		return -1;

	name = get_object_name(options, &tree->object);
	if (init_tree_desc_gently(&desc, tree->buffer, tree->size))
		return -1;
	while (tree_entry_gently(&desc, &entry)) {
		struct object *obj;
		int result;

		if (S_ISGITLINK(entry.mode))
			continue;

		if (S_ISDIR(entry.mode)) {
			obj = (struct object *)lookup_tree(entry.oid);
			if (name && obj)
				put_object_name(options, obj, "%s%s/", name,
					entry.path);
			result = options->walk(obj, OBJ_TREE, data, options);
		}
		else if (S_ISREG(entry.mode) || S_ISLNK(entry.mode)) {
			obj = (struct object *)lookup_blob(entry.oid);
			if (name && obj)
				put_object_name(options, obj, "%s%s", name,
					entry.path);
			result = options->walk(obj, OBJ_BLOB, data, options);
		}
		else {
			result = error("in tree %s: entry %s has bad mode %.6o",
					describe_object(options, &tree->object), entry.path, entry.mode);
		}
		if (result < 0)
			return result;
		if (!res)
			res = result;
	}
	return res;
}
