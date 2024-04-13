static int fsck_walk_commit(struct commit *commit, void *data, struct fsck_options *options)
{
	int counter = 0, generation = 0, name_prefix_len = 0;
	struct commit_list *parents;
	int res;
	int result;
	const char *name;

	if (parse_commit(commit))
		return -1;

	name = get_object_name(options, &commit->object);
	if (name)
		put_object_name(options, &commit->tree->object, "%s:", name);

	result = options->walk((struct object *)commit->tree, OBJ_TREE, data, options);
	if (result < 0)
		return result;
	res = result;

	parents = commit->parents;
	if (name && parents) {
		int len = strlen(name), power;

		if (len && name[len - 1] == '^') {
			generation = 1;
			name_prefix_len = len - 1;
		}
		else { /* parse ~<generation> suffix */
			for (generation = 0, power = 1;
			     len && isdigit(name[len - 1]);
			     power *= 10)
				generation += power * (name[--len] - '0');
			if (power > 1 && len && name[len - 1] == '~')
				name_prefix_len = len - 1;
		}
	}

	while (parents) {
		if (name) {
			struct object *obj = &parents->item->object;

			if (++counter > 1)
				put_object_name(options, obj, "%s^%d",
					name, counter);
			else if (generation > 0)
				put_object_name(options, obj, "%.*s~%d",
					name_prefix_len, name, generation + 1);
			else
				put_object_name(options, obj, "%s^", name);
		}
		result = options->walk((struct object *)parents->item, OBJ_COMMIT, data, options);
		if (result < 0)
			return result;
		if (!res)
			res = result;
		parents = parents->next;
	}
	return res;
}
