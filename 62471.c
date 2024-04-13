static void parse_options(struct exfat* ef, const char* options)
{
	int opt_umask;

	opt_umask = get_int_option(options, "umask", 8, 0);
	ef->dmask = get_int_option(options, "dmask", 8, opt_umask);
	ef->fmask = get_int_option(options, "fmask", 8, opt_umask);

	ef->uid = get_int_option(options, "uid", 10, geteuid());
	ef->gid = get_int_option(options, "gid", 10, getegid());

	ef->noatime = match_option(options, "noatime");
}
