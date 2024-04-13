static boolean t1_open_fontfile(int open_name_prefix)
{
    ff_entry *ff;
    int callback_id = 0;
    int file_opened = 0;
    t1_curbyte = 0;
    t1_size = 0;
    ff = check_ff_exist(fd_cur->fm->ff_name, is_truetype(fd_cur->fm));
    if (ff->ff_path == NULL) {
        formatted_error("type 1","cannot open file for reading '%s'",fd_cur->fm->ff_name);
        return false;
    }
    cur_file_name = luatex_find_file(ff->ff_path, find_type1_file_callback);
    if (cur_file_name == NULL) {
        formatted_error("type 1","cannot open file for reading '%s'", ff->ff_path);
        return false;
    }
    callback_id = callback_defined(read_type1_file_callback);
    if (callback_id > 0) {
        if (!run_callback(callback_id, "S->bSd", cur_file_name, &file_opened, &t1_buffer, &t1_size)
            && file_opened && t1_size > 0) {
            formatted_warning("type 1","cannot open file for reading '%s'",cur_file_name);
            return false;
        }
    } else {
        t1_file = xfopen(cur_file_name, FOPEN_RBIN_MODE);
        t1_read_file();
        t1_close();
    }
    recorder_record_input(cur_file_name);
    t1_init_params(open_name_prefix);
    return true;
}
