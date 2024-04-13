close_attr_file(ATTR_FILE_T * attr_file)
{
    if (attr_file == NULL) {
        tsk_error_set_errno(TSK_ERR_FS_READ);
        tsk_error_set_errstr("close_attr_file: NULL attr_file arg");
        return 1;
    }

    if (attr_file->file != NULL) {
        tsk_fs_file_close(attr_file->file);
        attr_file->file = NULL;
    }

    free(attr_file->header);
    attr_file->header = NULL;

    attr_file->rootNode = 0;
    attr_file->nodeSize = 0;
    return 0;
}
