hfs_extents_to_attr(TSK_FS_INFO * a_fs, const hfs_ext_desc * a_extents,
    TSK_OFF_T a_start_off)
{
    TSK_FS_ATTR_RUN *head_run = NULL;
    TSK_FS_ATTR_RUN *prev_run = NULL;
    int i;
    TSK_OFF_T cur_off = a_start_off;

    tsk_error_reset();

    if (tsk_verbose)
        tsk_fprintf(stderr,
            "hfs_extents_to_attr: Converting extents from offset %" PRIuOFF
            " to runlist\n", a_start_off);

    for (i = 0; i < 8; ++i) {
        TSK_FS_ATTR_RUN *cur_run;

        uint32_t addr = tsk_getu32(a_fs->endian, a_extents[i].start_blk);
        uint32_t len = tsk_getu32(a_fs->endian, a_extents[i].blk_cnt);

        if (tsk_verbose)
            tsk_fprintf(stderr,
                "hfs_extents_to_attr: run %i at addr %" PRIu32
                " with len %" PRIu32 "\n", i, addr, len);

        if ((addr == 0) && (len == 0)) {
            break;
        }

        if ((cur_run = tsk_fs_attr_run_alloc()) == NULL) {
            error_returned(" - hfs_extents_to_attr");
            return NULL;
        }

        cur_run->addr = addr;
        cur_run->len = len;
        cur_run->offset = cur_off;

        if (head_run == NULL)
            head_run = cur_run;
        if (prev_run != NULL)
            prev_run->next = cur_run;
        cur_off += cur_run->len;
        prev_run = cur_run;
    }

    return head_run;
}
