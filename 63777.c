static void t1_init_params(const char *open_name_prefix)
{
    t1_log(open_name_prefix);
    t1_log(cur_file_name);
    t1_lenIV = 4;
    t1_dr = 55665;
    t1_er = 55665;
    t1_in_eexec = 0;
    t1_cs = false;
    t1_scan = true;
    t1_synthetic = false;
    t1_eexec_encrypt = false;
    t1_block_length = 0;
    t1_check_pfa();
}
