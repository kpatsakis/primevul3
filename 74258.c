_rsvg_io_acquire_stream (const char *href, 
                         const char *base_uri, 
                         char **mime_type,
                         GCancellable *cancellable,
                         GError **error)
{
    GInputStream *stream;
    char *data;
    gsize len;

    if (!(href && *href)) {
        g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                            "Invalid URI");
        return NULL;
    }

    if (strncmp (href, "data:", 5) == 0) {
        if (!(data = rsvg_acquire_data_data (href, NULL, mime_type, &len, error)))
            return NULL;

        return g_memory_input_stream_new_from_data (data, len, (GDestroyNotify) g_free);
    }

    if ((data = rsvg_acquire_file_data (href, base_uri, mime_type, &len, cancellable, NULL)))
      return g_memory_input_stream_new_from_data (data, len, (GDestroyNotify) g_free);

    if ((stream = rsvg_acquire_gvfs_stream (href, base_uri, mime_type, cancellable, error)))
      return stream;

    return NULL;
}
