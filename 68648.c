WM_SYMBOL int WildMidi_GetOutput(midi * handle, int8_t *buffer, uint32_t size) {
    if (__builtin_expect((!WM_Initialized), 0)) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_INIT, NULL, 0);
        return (-1);
    }
    if (__builtin_expect((handle == NULL), 0)) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL handle)", 0);
        return (-1);
    }
    if (__builtin_expect((buffer == NULL), 0)) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL buffer pointer)", 0);
        return (-1);
    }
    if (__builtin_expect((size == 0), 0)) {
        return (0);
    }
    if (__builtin_expect((!!(size % 4)), 0)) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(size not a multiple of 4)", 0);
        return (-1);
    }

    if (((struct _mdi *) handle)->extra_info.mixer_options & WM_MO_ENHANCED_RESAMPLING) {
        if (!gauss_table) init_gauss();
        return (WM_GetOutput_Gauss(handle, buffer, size));
    }
    return (WM_GetOutput_Linear(handle, buffer, size));
}
