am_file_data_t *am_file_data_new(apr_pool_t *pool, const char *path)
{
    am_file_data_t *file_data = NULL;

    if ((file_data = apr_pcalloc(pool, sizeof(am_file_data_t))) == NULL) {
        return NULL;
    }

    file_data->pool = pool;
    file_data->rv = APR_EINIT;
    if (path) {
        file_data->path = apr_pstrdup(file_data->pool, path);
    }

    return file_data;
}
