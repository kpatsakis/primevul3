rsvg_acquire_gvfs_data (const char *uri,
                        const char *base_uri,
                        char **out_mime_type,
                        gsize *out_len,
                        GCancellable *cancellable,
                        GError **error)
{
    GFile *base, *file;
    GError *err;
    char *data;
    gsize len;
    char *content_type;
    gboolean res;

    file = g_file_new_for_uri (uri);

    err = NULL;
    data = NULL;
    if (!(res = g_file_load_contents (file, cancellable, &data, &len, NULL, &err)) &&
        g_error_matches (err, G_IO_ERROR, G_IO_ERROR_NOT_FOUND) &&
        base_uri != NULL) {
        g_clear_error (&err);
        g_object_unref (file);

        base = g_file_new_for_uri (base_uri);
        file = g_file_resolve_relative_path (base, uri);
        g_object_unref (base);

        res = g_file_load_contents (file, cancellable, &data, &len, NULL, &err);
    }

    g_object_unref (file);

    if (err) {
        g_propagate_error (error, err);
        return NULL;
    }

    if (out_mime_type &&
        (content_type = g_content_type_guess (uri, (guchar *) data, len, NULL))) {
        *out_mime_type = g_content_type_get_mime_type (content_type);
        g_free (content_type);
    }

    *out_len = len;
    return data;
}
