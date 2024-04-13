WM_SYMBOL int WildMidi_SetCvtOption(uint16_t tag, uint16_t setting) {
    _WM_Lock(&WM_ConvertOptions.lock);
    switch (tag) {
    case WM_CO_XMI_TYPE: /* validation happens in xmidi.c */
        WM_ConvertOptions.xmi_convert_type = setting;
        break;
    case WM_CO_FREQUENCY: /* validation happens in format */
        WM_ConvertOptions.frequency = setting;
        break;
    default:
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(invalid setting)", 0);
        _WM_Unlock(&WM_ConvertOptions.lock);
        return (-1);
    }
    _WM_Unlock(&WM_ConvertOptions.lock);
    return (0);
}
