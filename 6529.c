u32 parse_compress(char *arg_val, u32 opt)
{
	compress_moov = opt ? 2 : 1;
	open_edit = GF_TRUE;
	return 0;
}