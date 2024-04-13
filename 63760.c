static void cs_init(void)
{
    cs_ptr = cs_tab = NULL;
    cs_dict_start = cs_dict_end = NULL;
    cs_count = cs_size = cs_size_pos = 0;
    cs_token_pair = NULL;
    subr_tab = NULL;
    subr_array_start = subr_array_end = NULL;
    subr_max = subr_size = subr_size_pos = 0;
}
