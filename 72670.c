void do_flatten_cache(const char *cache_dir)
{
	gf_enum_directory(cache_dir, GF_FALSE, revert_cache_file, NULL, "*.txt");
}
