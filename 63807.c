static void t1_close_font_file(int close_name_suffix)
{
    report_stop_file(close_name_suffix);
    cur_file_name = NULL;
}
