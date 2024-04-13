WM_SYMBOL int WildMidi_MasterVolume(uint8_t master_volume) {
    if (!WM_Initialized) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_INIT, NULL, 0);
        return (-1);
    }
    if (master_volume > 127) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG,
                "(master volume out of range, range is 0-127)", 0);
        return (-1);
    }

    _WM_MasterVolume = _WM_lin_volume[master_volume];

    return (0);
}
