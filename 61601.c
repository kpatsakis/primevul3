_gids_hashtbl_idx(const char *user)
{
	uint64_t x = siphash_str(user);
	return x % GIDS_HASH_LEN;
}
