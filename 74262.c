rsvg_acquire_gvfs_stream (const char *uri, 
                          const char *base_uri, 
                          char **out_mime_type,
                          GCancellable *cancellable,
                          GError **error)
{
    GFile *base, *file;
    GFileInputStream *stream;
    GError *err = NULL;

    file = g_file_new_for_uri (uri);

    stream = g_file_read (file, cancellable, &err);
    g_object_unref (file);

    if (stream == NULL &&
        g_error_matches (err, G_IO_ERROR, G_IO_ERROR_NOT_FOUND)) {
        g_clear_error (&err);

        base = g_file_new_for_uri (base_uri);
        file = g_file_resolve_relative_path (base, uri);
        g_object_unref (base);

        stream = g_file_read (file, cancellable, &err);
        g_object_unref (file);
    }

    if (stream == NULL) {
        g_propagate_error (error, err);
        return NULL;
    }

    if (out_mime_type) {
        GFileInfo *file_info;
        const char *content_type;

        file_info = g_file_input_stream_query_info (stream, 
                                                    G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE,
                                                    cancellable,
                                                    NULL /* error */);
        if (file_info &&
            (content_type = g_file_info_get_content_type (file_info)))
            *out_mime_type = g_content_type_get_mime_type (content_type);
        else
            *out_mime_type = NULL;

        if (file_info)
            g_object_unref (file_info);
    }

    return G_INPUT_STREAM (stream);
}
