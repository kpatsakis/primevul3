uint16_t _cvt_get_option (uint16_t tag) {
    uint16_t r = 0;
    _WM_Lock(&WM_ConvertOptions.lock);
    switch (tag) {
    case WM_CO_XMI_TYPE: r = WM_ConvertOptions.xmi_convert_type; break;
    case WM_CO_FREQUENCY: r = WM_ConvertOptions.frequency; break;
    }
    _WM_Unlock(&WM_ConvertOptions.lock);
    return r;
}
