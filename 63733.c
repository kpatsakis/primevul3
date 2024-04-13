int fsck_error_function(struct fsck_options *o,
	struct object *obj, int msg_type, const char *message)
{
	if (msg_type == FSCK_WARN) {
		warning("object %s: %s", describe_object(o, obj), message);
		return 0;
	}
	error("object %s: %s", describe_object(o, obj), message);
	return 1;
}
