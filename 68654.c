WM_SYMBOL int WildMidi_Shutdown(void) {
    if (!WM_Initialized) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_NOT_INIT, NULL, 0);
        return (-1);
    }
    while (first_handle) {
        /* closes open handle and rotates the handles list. */
        WildMidi_Close((struct _mdi *) first_handle->handle);
    }
    WM_FreePatches();
    free_gauss();

    /* reset the globals */
    _cvt_reset_options ();
    _WM_MasterVolume = 948;
    _WM_MixerOptions = 0;
    _WM_fix_release = 0;
    _WM_auto_amp = 0;
    _WM_auto_amp_with_amp = 0;
    _WM_reverb_room_width = 16.875f;
    _WM_reverb_room_length = 22.5f;
    _WM_reverb_listen_posx = 8.4375f;
    _WM_reverb_listen_posy = 16.875f;

    WM_Initialized = 0;

    if (_WM_Global_ErrorS != NULL) free(_WM_Global_ErrorS);

    return (0);
}
