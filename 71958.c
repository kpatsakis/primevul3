static const char *am_request_hostname(request_rec *r)
{
    const char *url;
    apr_uri_t uri;
    int ret;

    url = am_reconstruct_url(r);

    ret = apr_uri_parse(r->pool, url, &uri);
    if (ret != APR_SUCCESS) {
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                      "Failed to parse request URL: %s", url);
        return NULL;
    }

    if (uri.hostname == NULL) {
        /* This shouldn't happen, since the request URL is built with a hostname,
         * but log a message to make any debuggin around this code easier.
         */
        AM_LOG_RERROR(APLOG_MARK, APLOG_ERR, 0, r,
                      "No hostname in request URL: %s", url);
        return NULL;
    }

    return uri.hostname;
}
