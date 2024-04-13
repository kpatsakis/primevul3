WM_SYMBOL int WildMidi_Close(midi * handle) {
    struct _mdi *mdi = (struct _mdi *) handle;
    struct _hndl * tmp_handle;

    if (!WM_Initialized) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_INIT, NULL, 0);
        return (-1);
    }
    if (handle == NULL) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL handle)", 0);
        return (-1);
    }
    if (first_handle == NULL) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(no midi's open)", 0);
        return (-1);
    }
    _WM_Lock(&mdi->lock);
    if (first_handle->handle == handle) {
        tmp_handle = first_handle->next;
        free(first_handle);
        first_handle = tmp_handle;
        if (first_handle)
            first_handle->prev = NULL;
    } else {
        tmp_handle = first_handle;
        while (tmp_handle->handle != handle) {
            tmp_handle = tmp_handle->next;
            if (tmp_handle == NULL) {
                break;
            }
        }
        if (tmp_handle) {
            tmp_handle->prev->next = tmp_handle->next;
            if (tmp_handle->next) {
                tmp_handle->next->prev = tmp_handle->prev;
            }
            free(tmp_handle);
        }
    }

    _WM_freeMDI(mdi);

    return (0);
}
