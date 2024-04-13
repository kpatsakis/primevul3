static void uncurl_default_opts(struct uncurl_opts *opts)
{
	opts->max_header = 1024;
	opts->max_body = 128 * 1024 * 1024;
}
