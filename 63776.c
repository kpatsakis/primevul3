static void t1_close_font_file(const char *close_name_suffix)
{
    t1_log(close_name_suffix);
    t1_close();
    cur_file_name = NULL;
}
