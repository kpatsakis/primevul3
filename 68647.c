WM_SYMBOL int WildMidi_GetMidiOutput(midi * handle, int8_t **buffer, uint32_t *size) {
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
    return _WM_Event2Midi(handle, (uint8_t **)buffer, size);
}
