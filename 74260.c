rsvg_acquire_file_data (const char *filename,
                        const char *base_uri,
                        char **out_mime_type,
                        gsize *out_len,
                        GCancellable *cancellable,
                        GError **error)
{
    gchar *path, *data;
    gsize len;
    char *content_type;

    rsvg_return_val_if_fail (filename != NULL, NULL, error);
    g_assert (out_len != NULL);

    path = _rsvg_io_get_file_path (filename, base_uri);
    if (path == NULL)
        return NULL;

    if (!g_file_get_contents (path, &data, &len, error)) {
        g_free (path);
        return NULL;
    }

    if (out_mime_type &&
        (content_type = g_content_type_guess (path, (guchar *) data, len, NULL))) {
        *out_mime_type = g_content_type_get_mime_type (content_type);
        g_free (content_type);
    }

    g_free (path);

    *out_len = len;
    return data;
}
