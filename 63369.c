void WavpackSetFileInformation (WavpackContext *wpc, char *file_extension, unsigned char file_format)
{
    if (file_extension && strlen (file_extension) < sizeof (wpc->file_extension)) {
        add_to_metadata (wpc, file_extension, (uint32_t) strlen (file_extension), ID_ALT_EXTENSION);
        strcpy (wpc->file_extension, file_extension);
    }

    wpc->file_format = file_format;
}
