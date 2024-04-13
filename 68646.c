WM_SYMBOL char * WildMidi_GetLyric (midi * handle) {
    struct _mdi *mdi = (struct _mdi *) handle;
    char * lyric = NULL;

    if (!WM_Initialized) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_INIT, NULL, 0);
        return (NULL);
    }
    if (handle == NULL) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL handle)", 0);
        return (NULL);
    }
    _WM_Lock(&mdi->lock);
    lyric = mdi->lyric;
    mdi->lyric = NULL;
    _WM_Unlock(&mdi->lock);
    return (lyric);
}
