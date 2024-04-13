WM_SYMBOL int WildMidi_SetOption(midi * handle, uint16_t options, uint16_t setting) {
    struct _mdi *mdi;

    if (!WM_Initialized) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_INIT, NULL, 0);
        return (-1);
    }
    if (handle == NULL) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL handle)", 0);
        return (-1);
    }

    mdi = (struct _mdi *) handle;
    _WM_Lock(&mdi->lock);
    if ((!(options & 0x800F)) || (options & 0x7FF0)) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(invalid option)", 0);
        _WM_Unlock(&mdi->lock);
        return (-1);
    }
    if (setting & 0x7FF0) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(invalid setting)", 0);
        _WM_Unlock(&mdi->lock);
        return (-1);
    }

    mdi->extra_info.mixer_options = ((mdi->extra_info.mixer_options & (0x80FF ^ options))
                                    | (options & setting));

    if (options & WM_MO_LOG_VOLUME) {
            _WM_AdjustChannelVolumes(mdi, 16);  // Settings greater than 15
    } else if (options & WM_MO_REVERB) {
        _WM_reset_reverb(mdi->reverb);
    }

    _WM_Unlock(&mdi->lock);
    return (0);
}
