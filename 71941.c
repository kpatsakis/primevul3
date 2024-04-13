apr_status_t am_file_read(am_file_data_t *file_data)
{
    char buffer[512];
    apr_file_t *fd;
    apr_size_t nbytes;

    if (file_data == NULL) {
        return APR_EINVAL;
    }
    file_data->rv = APR_SUCCESS;
    file_data->strerror = NULL;

    am_file_stat(file_data);
    if (file_data->rv != APR_SUCCESS) {
        return file_data->rv;
    }

    if ((file_data->rv = apr_file_open(&fd, file_data->path,
                                       APR_READ, 0, file_data->pool)) != 0) {
        file_data->strerror =
            apr_psprintf(file_data->pool,
                         "apr_file_open: Error opening \"%s\" [%d] \"%s\"",
                         file_data->path, file_data->rv,
                         apr_strerror(file_data->rv, buffer, sizeof(buffer)));
        return file_data->rv;
    }

    file_data->read_time = apr_time_now();
    nbytes = file_data->finfo.size;
    file_data->contents = (char *)apr_palloc(file_data->pool, nbytes + 1);

    file_data->rv = apr_file_read_full(fd, file_data->contents, nbytes, NULL);
    if (file_data->rv != 0) {
        file_data->strerror =
            apr_psprintf(file_data->pool,
                         "apr_file_read_full: Error reading \"%s\" [%d] \"%s\"",
                         file_data->path, file_data->rv,
                         apr_strerror(file_data->rv, buffer, sizeof(buffer)));
        (void)apr_file_close(fd);
        return file_data->rv;

    }
    file_data->contents[nbytes] = '\0';

    (void)apr_file_close(fd);

    return file_data->rv;
}
