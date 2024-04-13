static int index_error_invalid(const char *message)
{
	giterr_set(GITERR_INDEX, "invalid data in index - %s", message);
	return -1;
}
