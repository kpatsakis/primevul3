WM_SYMBOL const char *WildMidi_GetString(uint16_t info) {
    static char WM_Version[] = "WildMidi Processing Library " PACKAGE_VERSION;
    switch (info) {
    case WM_GS_VERSION:
        return WM_Version;
    }
    return NULL;
}
