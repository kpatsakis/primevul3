apr_status_t am_file_stat(am_file_data_t *file_data)
{
    char buffer[512];

    if (file_data == NULL) {
        return APR_EINVAL;
    }

    file_data->strerror = NULL;

    file_data->stat_time = apr_time_now();
    file_data->rv = apr_stat(&file_data->finfo, file_data->path,
                             APR_FINFO_SIZE, file_data->pool);
    if (file_data->rv != APR_SUCCESS) {
        file_data->strerror =
            apr_psprintf(file_data->pool,
                         "apr_stat: Error opening \"%s\" [%d] \"%s\"",
                         file_data->path, file_data->rv,
                         apr_strerror(file_data->rv, buffer, sizeof(buffer)));
    }

    return file_data->rv;
}
