_rsvg_io_acquire_data (const char *href, 
                       const char *base_uri, 
                       char **mime_type,
                       gsize *len,
                       GCancellable *cancellable,
                       GError **error)
{
    char *data;
    gsize llen;

    if (!(href && *href)) {
        g_set_error_literal (error, G_IO_ERROR, G_IO_ERROR_FAILED,
                            "Invalid URI");
        return NULL;
    }

    if (!len)
        len = &llen;

    if (strncmp (href, "data:", 5) == 0)
      return rsvg_acquire_data_data (href, NULL, mime_type, len, error);

    if ((data = rsvg_acquire_file_data (href, base_uri, mime_type, len, cancellable, NULL)))
      return data;

    if ((data = rsvg_acquire_gvfs_data (href, base_uri, mime_type, len, cancellable, error)))
      return data;

    return NULL;
}
