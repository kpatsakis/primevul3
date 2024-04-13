static int create_index_error(int error, const char *msg)
{
	giterr_set_str(GITERR_INDEX, msg);
	return error;
}
