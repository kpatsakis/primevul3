static boolean t1_open_fontfile(const char *open_name_prefix)
{
    if (!t1_open()) {
       char *msg = concat ("! Couldn't find font file ", cur_file_name);
       error(msg);
    }
    t1_init_params(open_name_prefix);
    return true;                /* font file found */
}
