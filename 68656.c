static int add_handle(void * handle) {
    struct _hndl *tmp_handle = NULL;

    if (first_handle == NULL) {
        first_handle = malloc(sizeof(struct _hndl));
        if (first_handle == NULL) {
            _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_MEM, " to get ram", errno);
            return (-1);
        }
        first_handle->handle = handle;
        first_handle->prev = NULL;
        first_handle->next = NULL;
    } else {
        tmp_handle = first_handle;
        if (tmp_handle->next) {
            while (tmp_handle->next)
                tmp_handle = tmp_handle->next;
        }
        tmp_handle->next = malloc(sizeof(struct _hndl));
        if (tmp_handle->next == NULL) {
            _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_MEM, " to get ram", errno);
            return (-1);
        }
        tmp_handle->next->prev = tmp_handle;
        tmp_handle = tmp_handle->next;
        tmp_handle->next = NULL;
        tmp_handle->handle = handle;
    }
    return (0);
}
