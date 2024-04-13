static void free_gauss(void) {
    _WM_Lock(&gauss_lock);
    free(gauss_table);
    gauss_table = NULL;
    _WM_Unlock(&gauss_lock);
}
