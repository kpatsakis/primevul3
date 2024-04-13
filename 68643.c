WM_SYMBOL int WildMidi_ConvertToMidi (const char *file, uint8_t **out, uint32_t *size) {
    uint8_t *buf;
    int ret;

    if (!file) {
        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_INVALID_ARG, "(NULL filename)", 0);
        return (-1);
    }
    if ((buf = (uint8_t *) _WM_BufferFile(file, size)) == NULL) {
        return (-1);
    }

    ret = WildMidi_ConvertBufferToMidi(buf, *size, out, size);
    free(buf);
    return ret;
}
