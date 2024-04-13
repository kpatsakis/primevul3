WM_SYMBOL void WildMidi_ClearError (void) {
    _WM_Global_ErrorI = 0;
    if (_WM_Global_ErrorS != NULL) {
        free(_WM_Global_ErrorS);
        _WM_Global_ErrorS = NULL;
    }
    return;
}
