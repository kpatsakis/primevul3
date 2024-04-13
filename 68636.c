static void WM_FreePatches(void) {
    int i;
    struct _patch * tmp_patch;
    struct _sample * tmp_sample;

    _WM_Lock(&_WM_patch_lock);
    for (i = 0; i < 128; i++) {
        while (_WM_patch[i]) {
            while (_WM_patch[i]->first_sample) {
                tmp_sample = _WM_patch[i]->first_sample->next;
                free(_WM_patch[i]->first_sample->data);
                free(_WM_patch[i]->first_sample);
                _WM_patch[i]->first_sample = tmp_sample;
            }
            free(_WM_patch[i]->filename);
            tmp_patch = _WM_patch[i]->next;
            free(_WM_patch[i]);
            _WM_patch[i] = tmp_patch;
        }
    }
    _WM_Unlock(&_WM_patch_lock);
}
