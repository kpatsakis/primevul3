WildMidi_GetInfo(midi * handle) {
    struct _mdi *mdi = (struct _mdi *) handle;
    if (!WM_Initialized) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_INIT, NULL, 0);
        return (NULL);
    }
    if (handle == NULL) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL handle)", 0);
        return (NULL);
    }
    _WM_Lock(&mdi->lock);
    if (mdi->tmp_info == NULL) {
        mdi->tmp_info = malloc(sizeof(struct _WM_Info));
        if (mdi->tmp_info == NULL) {
            _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_MEM, "to set info", 0);
            _WM_Unlock(&mdi->lock);
            return (NULL);
        }
        mdi->tmp_info->copyright = NULL;
    }
    mdi->tmp_info->current_sample = mdi->extra_info.current_sample;
    mdi->tmp_info->approx_total_samples = mdi->extra_info.approx_total_samples;
    mdi->tmp_info->mixer_options = mdi->extra_info.mixer_options;
    mdi->tmp_info->total_midi_time = (mdi->tmp_info->approx_total_samples * 1000) / _WM_SampleRate;
    if (mdi->extra_info.copyright) {
        free(mdi->tmp_info->copyright);
        mdi->tmp_info->copyright = malloc(strlen(mdi->extra_info.copyright) + 1);
        if (mdi->tmp_info->copyright == NULL) {
            free(mdi->tmp_info);
            mdi->tmp_info = NULL;
            _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_MEM, "to set copyright", 0);
            _WM_Unlock(&mdi->lock);
            return (NULL);
        } else {
            strcpy(mdi->tmp_info->copyright, mdi->extra_info.copyright);
        }
    } else {
        mdi->tmp_info->copyright = NULL;
    }
    _WM_Unlock(&mdi->lock);
    return ((struct _WM_Info *)mdi->tmp_info);
}
